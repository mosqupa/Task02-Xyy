#include "utils.hpp"
#include "armour.hpp"
#include "armor.hpp"


int main(){
    // 读取图像
    Mat img = imread("../resources/test_image.png");
    Mat img2 = imread("../resources/test_image_2.png");
    if (img.empty() || img2.empty()) {
        cout << "无法读取图像!" << endl;
        return -1;
    }

    basicOperation(img); // 基础操作
    detectArmour(img2); // 装甲板识别

    return 0;
}
