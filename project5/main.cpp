#include "cnn.hpp"

int main(int argc, char const* argv[]) {
    cv::Mat img = cv::imread("./samples/bg.jpg");
    cv::imshow("test", img);
    cv::waitKey();
    return 0;
}
