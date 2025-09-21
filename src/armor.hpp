#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;

enum {
    RED,
    BLUE
};

// 降低图像曝光
Mat reduceBrightness(const Mat& src, Scalar rgbLower, Scalar rgbUpper, float factor = 0.3f);

// 剔除颜色
Mat seperateColors(const Mat& img, int enemy_color = 1);

// 二值+膨胀
Mat toBinaryAndDilate(const Mat& grayImg, int brightness_threshold = 190);

// 获取轮廓
vector<vector<Point>> getContours(const Mat& binBrightImg);

// 检测
void detectArmour(const Mat& img);