#include "cnn.hpp"

int main(int argc, char const* argv[]) {
    cv::Mat img = cv::imread("../samples/bg.jpg");
    cout << img << endl;
    return 0;
}
