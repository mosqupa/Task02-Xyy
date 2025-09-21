#include "armor.hpp"

Mat reduceBrightness(const Mat& src, Scalar rgbLower, Scalar rgbUpper, float factor) {
    Mat result = src.clone();
    Mat mask;
    inRange(src, rgbLower, rgbUpper, mask);
    
    // 对非掩码区域降低亮度
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            // 处理不在目标RGB范围内的像素
            if (mask.at<uchar>(y, x) == 0) {
                Vec3b& pixel = result.at<Vec3b>(y, x);
                // 降低亮度
                pixel[0] = saturate_cast<uchar>(pixel[0] * factor);  // B通道
                pixel[1] = saturate_cast<uchar>(pixel[1] * factor);  // G通道
                pixel[2] = saturate_cast<uchar>(pixel[2] * factor);  // R通道
            }
        }
    }
    
    return result;
}

Mat seperateColors(const Mat& img, int enemy_color) {
    vector<Mat> channels;
    Mat roiImg = img.clone();
    Mat grayImg;
    split(roiImg, channels); // 把一个3通道图像转换成3个单通道图像
    if (enemy_color==RED) {
        grayImg = channels.at(2)-channels.at(0); // R-B
    }
    else {
        grayImg = channels.at(0)-channels.at(2); // B-R
    }
    return grayImg;
}

Mat toBinaryAndDilate(const Mat& grayImg, int brightness_threshold) {
    Mat binBrightImg;
    threshold(grayImg, binBrightImg, brightness_threshold, 255, THRESH_BINARY);
    Mat element = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
    dilate(binBrightImg, binBrightImg, element);
    // morphologyEx(binBrightImg, binBrightImg, MORPH_CLOSE, element);
    return binBrightImg;
}

vector<vector<Point>> getContours(const Mat& binBrightImg) {
    vector<vector<Point>> lightContours;
    vector<Vec4i> hierarchy;
    findContours(binBrightImg.clone(),lightContours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    return lightContours;
}

void detectArmour(const Mat& img) {
    // 设置BGR筛选范围
    Scalar rgbLower(90, 10, 0);
    Scalar rgbUpper(255, 250, 50);

    Mat result = reduceBrightness(img, rgbLower, rgbUpper, 0.2f);
    Mat grayimg = seperateColors(result, 1);
    Mat binBrightImg = toBinaryAndDilate(grayimg);
    vector<vector<Point>> contours = getContours(binBrightImg);
    Mat drawImg = img.clone();

    vector<Rect> lights;
    for (size_t i = 0; i < contours.size(); i++) {
        double area = contourArea(contours[i]);
        if (area < 2000) continue; //面积过小的剔除
        Rect bbox = boundingRect(contours[i]);
        // rectangle(drawImg, bbox, Scalar(0, 0, 255), 2);
        lights.push_back(bbox);
    }

    for (size_t i = 0; i < lights.size(); i++) {
        for (size_t j = i+1; j < lights.size(); j++) {
            Rect r1 = lights[i];
            Rect r2 = lights[j];

            // 判断是否可能是一对装甲灯条
            float height_ratio = (float)min(r1.height, r2.height) / (float)max(r1.height, r2.height);
            if (height_ratio < 0.6) continue; // 高度差太大，不可能是同一块装甲板

            int y_diff = abs(r1.y - r2.y);
            if (y_diff > max(r1.height, r2.height) * 0.5) continue; // 垂直差太大

            // 横向间距合理
            int x_diff = abs((r1.x + r1.width/2) - (r2.x + r2.width/2));
            if (x_diff < max(r1.width, r2.width)) continue; // 离得太近，不像一对灯条

            // 合并两个框，得到装甲板框
            Rect armor = r1 | r2; // 取两个矩形的并集
            rectangle(drawImg, armor, Scalar(0, 0, 255), 2);
        }
    }

    imwrite("../output/识别结果.png", drawImg);
}