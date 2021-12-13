#include "cnn_matrix.hpp"
#include "params.hpp"
#define DEBUG 1

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
        // cnn.add_layer(new SoftmaxLayer());

        M2D res = cnn.predict("../samples/face.jpg");
        res[0][0].print();

        return 0;
    }

    // DEBUG codes, I wanna keep them
    float a[2 * 3 * 3 * 3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    float b[1 * 3 * 3 * 3] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    float c[1 * 3 * 4 * 4] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    float d[1 * 3 * 4 * 4];
    for (int i = 0; i < 1 * 3 * 4 * 4; i++) {
        d[i] = (float)i;
    }

    float e[1 * 3 * 4 * 4];
    for (int i = 0; i < 1 * 3 * 4 * 4; i++) {
        e[i] = 1;
    }

    float weight[5 * 3 * 3 * 3];
    for (int i = 0; i < 5 * 3 * 3 * 3; i++) {
        weight[i] = 2;
    }

    float bias[5] = {-1.0f, -1.0f, -1.0f, -1.0f, -1.0f};

    M2D m2d = M2D(1, 3);
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < 3; j++) {
            float* m = d + i * (1 * 4 * 4) + j * (4 * 4);
            m2d[i][j] = Matrix<float>(4, 4, m);
        }
    }
    print_m2d(m2d);
    flatten(m2d).print();
    exit(0);

    int kernel_size = 3;
    int padding = 1;
    int stride = 1;

    ConvBNLayer cbl(padding, stride, kernel_size, 3, 5, weight, bias);
    m2d = cbl.conv_bn_m2d(m2d);
    print_m2d(m2d);

    ReLULayer rl;
    m2d = rl.relu_m2d(m2d);
    print_m2d(m2d);

    MaxPoolingLayer mpl(2, 2);
    m2d = mpl.max_pooling_m2d(m2d);
    print_m2d(m2d);

    float fc_weight[2 * 20];
    for (int i = 0; i < 2 * 20; i++) {
        fc_weight[i] = 3;
    }
    float fc_bias[2] = {-1, -1};

    FCLayer fcl(20, 2, fc_weight, fc_bias);
    m2d = fcl.fully_connect_m2d(m2d);
    print_m2d(m2d);  // [6439, 6439]

    // // SoftmaxLayer sml;
    // // m2d = sml.softmax_m2d(m2d);
    // // m2d.print();

    return 0;
}
