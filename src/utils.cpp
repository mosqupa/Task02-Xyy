#include "utils.hpp"

Mat cvtToGray(const Mat& img) {
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    return gray;
}

Mat cvtToHSV(const Mat& img) {
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);
    return hsv;
}

Mat applyMeanBlur(const Mat& img) {
    Mat blurImg;
    blur(img, blurImg, Size(5, 5));
    return blurImg;
}

Mat applyGaussianBlur(const Mat& img) {
    Mat gaussianImg;
    GaussianBlur(img, gaussianImg, Size(5, 5), 1.5);
    return gaussianImg;
}

Mat getRedRegion(const Mat& img) {
    Mat hsv, mask, mask1, mask2;
    cvtColor(img, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(0, 50, 50), Scalar(20, 255, 255), mask1); // 范围1
    inRange(hsv, Scalar(160, 50, 50), Scalar(180, 255, 255), mask2); // 范围2
    mask = mask1 | mask2;  // 合并
    return mask;
}

vector<vector<Point>> getContoursFromMask(const Mat& mask) {
    vector<vector<Point>> contours;
    Mat morph;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(mask, morph, MORPH_CLOSE, kernel); // 闭运算连接断裂区域
    
    findContours(morph, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    return contours;
}

vector<Rect> getBoundingBoxes(const vector<vector<Point>>& contours, double minArea) {
    vector<Rect> boxes;
    cout << "红色外轮廓面积：" <<endl;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area >= minArea) {
            cout << area << endl;
            boxes.push_back(boundingRect(contours[i]));
        }
    }
    return boxes;
}

void processHighlightRegion(Mat& img) {
    Mat gray, binary, dilated, eroded;
    
    // 灰度化
    cvtColor(img, gray, COLOR_BGR2GRAY);
    showImage("高亮-灰度", gray);
    
    // 二值化
    threshold(gray, binary, 200, 255, THRESH_BINARY);
    showImage("高亮-二值化", binary);
    
    // 膨胀
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(binary, dilated, kernel);
    showImage("高亮-膨胀", dilated);
    
    // 腐蚀
    erode(binary, eroded, kernel);
    showImage("高亮-腐蚀", eroded);
    
    // 漫水填充
    Mat flood = binary.clone();
    Point seedPoint(200, 900);
    floodFill(flood, seedPoint, Scalar(255));
    showImage("高亮-漫水填充", flood);
}

void drawShapesAndText(Mat& img) {
    // 绘制圆形
    circle(img, Point(100, 100), 50, Scalar(255, 0, 0), 2);
    
    // 绘制矩形
    rectangle(img, Point(200, 200), Point(300, 300), Scalar(0, 255, 0), 2);
    
    // 绘制文字
    putText(img, "Task02 xyy", Point(150, 400), 
            FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
}

Mat rotateImage(const Mat& img, double angle) {
    Point2f center(img.cols / 2.0, img.rows / 2.0);
    Mat rotMat = getRotationMatrix2D(center, angle, 1.0);
    Mat rotated;
    warpAffine(img, rotated, rotMat, img.size());
    return rotated;
}

Mat cropImage(const Mat& img, double ratio) {
    int width = img.cols * ratio;
    int height = img.rows * ratio;
    Rect roi(0, 0, width, height);
    return img(roi).clone();
}

void showImage(const string& name, const Mat& img, bool wait, int time) {
    imwrite("../output/" + name + ".png", img);
    if (wait) {
        imshow(name, img);
        waitKey(time);
        destroyWindow(name); 
    }
}

void basicOperation(const Mat& src) {
    Mat img = src.clone();
    // 颜色空间转换
    Mat gray = cvtToGray(img);
    Mat hsv = cvtToHSV(img);
    showImage("灰度图", gray);
    showImage("HSV图", hsv);

    // 滤波操作
    Mat meanBlurImg = applyMeanBlur(img);
    Mat gaussianBlurImg = applyGaussianBlur(img);
    showImage("均值滤波", meanBlurImg);
    showImage("高斯滤波", gaussianBlurImg);

    // 提取红色区域
    Mat redMask = getRedRegion(img);
    showImage("红色掩膜", redMask);

    // 寻找轮廓和边界框
    vector<vector<Point>> contours = getContoursFromMask(redMask);
    vector<Rect> boxes = getBoundingBoxes(contours);

    // 在原图上绘制轮廓和边界框
    Mat resultImg = img.clone();
    for (size_t i = 0; i < contours.size(); i++) {
        drawContours(resultImg, contours, (int)i, Scalar(0, 255, 0), 2);
    }
    for (size_t i = 0; i < boxes.size(); i++) {
        rectangle(resultImg, boxes[i], Scalar(0, 0, 255), 2);
    }
    showImage("红色区域检测", resultImg);

    // 高亮颜色区域处理
    processHighlightRegion(img);

    // 绘制文字与图形
    Mat drawImg = img.clone();
    drawShapesAndText(drawImg);
    showImage("图形绘制", drawImg);

    // 图像旋转与裁剪
    Mat rotated = rotateImage(img, 35);
    Mat cropped = cropImage(img);
    showImage("旋转35度", rotated);
    showImage("裁剪1/4", cropped);
}
