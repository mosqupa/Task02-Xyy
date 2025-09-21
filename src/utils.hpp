#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

Mat cvtToGray(const Mat& img); // 转成灰度图
Mat cvtToHSV(const Mat& img); // 转成HSV
Mat applyMeanBlur(const Mat& img); // 均值滤波
Mat applyGaussianBlur(const Mat& img); // 高斯滤波
Mat getRedRegion(const Mat& img); // 提取红色区域
vector<vector<Point>> getContoursFromMask(const Mat& mask); // 提取轮廓
vector<Rect> getBoundingBoxes(const vector<vector<Point>>& contours, double minArea = 500.0); // 提取bbox
void drawShapesAndText(Mat& img); // 绘制图形与文字
Mat rotateImage(const Mat& img, double angle); // 旋转图片
Mat cropImage(const Mat& img, double ratio = 0.25); // 裁剪图片
void processHighlightRegion(Mat& img); // 处理高亮区域
void showImage(const string& name, const Mat& img, bool wait = false, int time = 500);
void basicOperation(const Mat& img);
