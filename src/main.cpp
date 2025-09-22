#include "utils.hpp"
#include "armor.hpp"


int main(){
    // 读取图像
    Mat img = imread("../resources/test_image.png");
    Mat img2 = imread("../resources/test_image_2.png");
    if (img.empty() || img2.empty()) {
        cout << "无法读取图像!" << endl;
        return -1;
    }

    cout << "开始运行！" << endl;
    basicOperation(img); // 基础操作
    detectArmour(img2); // 装甲板识别
    cout << "运行结束, 图片结果已保存到output/" << endl;

    return 0;
}
