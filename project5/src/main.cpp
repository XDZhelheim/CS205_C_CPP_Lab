#include "cnn.hpp"
#include "params.hpp"
#define DEBUG 0

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char const* argv[]) {
    if (!DEBUG) {
        CNN cnn;

        cnn.add_layer(new ConvBNLayer(conv_params[0]));
        cnn.add_layer(new ReLULayer());
        cnn.add_layer(new MaxPoolingLayer(2, 2));
        cnn.add_layer(new ConvBNLayer(conv_params[1]));
        cnn.add_layer(new ReLULayer());
        cnn.add_layer(new MaxPoolingLayer(2, 2));
        cnn.add_layer(new ConvBNLayer(conv_params[2]));
        cnn.add_layer(new ReLULayer());
        cnn.add_layer(new FCLayer(fc_params[0]));
        cnn.add_layer(new SoftmaxLayer());

        Matrix2dArray<float> res = cnn.predict("../samples/face.jpg");
        res.base_mat.print();

        return 0;
    }

    // DEBUG codes, I wanna keep them
    int a[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int b[6] = {1, 2, 3, 4, 5, 6};
    int c[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int d[16] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

    float k[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    int k2[4] = {3, 3, 3, 3};
    Matrix<float> kernel(3, 3, k);
    Matrix<int> kernel2(2, 2, k2);

    Matrix<int> m4(4, 4, d);

    int kernel_size = 3;
    int padding = 1;
    int stride = 1;

    // 2dArray test
    float rgb[3 * 25];
    for (int i = 0; i < 3 * 25; i++) {
        rgb[i] = 2.0f;
    }

    float weight[5 * 3 * 3 * 3];
    for (int i = 0; i < 5 * 3 * 3 * 3; i++) {
        weight[i] = 3.0f;
    }

    float bias[5] = {-1.0f, -1.0f, -1.0f, -1.0f, -1.0f};

    Matrix2dArray<float> m2d(1, 3, 5, 5, rgb);

    ConvBNLayer cbl(padding, stride, kernel_size, 3, 5, weight, bias);
    m2d = cbl.conv_bn_m2d(m2d);
    m2d.print();

    ReLULayer rl;
    m2d = rl.relu_m2d(m2d);
    m2d.print();

    MaxPoolingLayer mpl(2, 2);
    m2d = mpl.max_pooling_m2d(m2d);
    m2d.print();

    float fc_weight[2 * 20];
    for (int i = 0; i < 2 * 20; i++) {
        fc_weight[i] = 2.0f;
    }
    float fc_bias[2] = {-1, -1};

    FCLayer fcl(20, 2, fc_weight, fc_bias);
    m2d = fcl.fully_connect_m2d(m2d);
    m2d.print();  // [6439, 6439]

    cv::Mat img = cv::imread("../samples/face.jpg");
    cv::Mat fimg;
    img.convertTo(fimg, CV_32FC3);
    cv::normalize(fimg, fimg);
    cout << fimg.channels() << endl;
    cout << fimg.size << endl;

    return 0;
}
