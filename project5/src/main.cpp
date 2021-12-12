#include "cnn.hpp"
#include "params.hpp"

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char const* argv[]) {
    // CNN cnn;
    // Matrix2dArray<float> res = cnn.predict("../samples/face.jpg");

    // res.print();

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
    // m2d.print();

    ReLULayer rl;
    m2d = rl.relu_m2d(m2d);
    // m2d.print();

    MaxPoolingLayer mpl(2, 2);
    m2d = mpl.max_pooling_m2d(m2d);
    // m2d.print();

    float fc_weight[2 * 20];
    for (int i = 0; i < 2 * 20; i++) {
        weight[i] = 2.0f;
    }
    float fc_bias[2] = {-1, -1};

    FCLayer fcl(20, 2, fc_weight, fc_bias);
    m2d = fcl.full_connect_m2d(m2d);
    m2d.print();

    return 0;
}
