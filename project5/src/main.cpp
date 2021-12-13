// #include "cnn_matrix.hpp"
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
        // cnn.add_layer(new SoftmaxLayer());

        Matrix2dArray<float> res = cnn.predict("../samples/face.jpg");
        res.base_mat.print();
        // res.base_mat.print("./temp.txt");

        // M2D res = cnn.predict("../samples/face.jpg");
        // res[0][0].print();

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

    // // float k[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    // // float k2[4] = {3, 3, 3, 3};
    // // Matrix<float> kernel(3, 3, k);
    // // Matrix<float> kernel2(2, 2, k2);

    // // Matrix<float> m4(4, 4, d);

    // int kernel_size = 3;
    // int padding = 1;
    // int stride = 1;

    // // 2dArray test
    // float rgb[3 * 25];
    // for (int i = 0; i < 3 * 25; i++) {
    //     rgb[i] = 2.0f;
    // }

    // float weight[5 * 3 * 3 * 3];
    // for (int i = 0; i < 5 * 3 * 3 * 3; i++) {
    //     weight[i] = (float)i;
    // }

    // float bias[5] = {-1.0f, -1.0f, -1.0f, -1.0f, -1.0f};

    // // Matrix2dArray<float> m2d(1, 3, 5, 5, rgb);
    // // Matrix2dArray<float> m2d(2, 3, 3, 3, Matrix2dArray<float>::convert_data_array(2, 3, 3, 3, a));
    // // Matrix2dArray<float> m2d(1, 3, 4, 4, Matrix2dArray<float>::convert_data_array(1, 3, 4, 4, d));

    // M2D m2d = M2D(1, 3);
    // for (int i = 0; i < 1; i++) {
    //     for (int j = 0; j < 3; j++) {
    //         float* m = d + i * (1 * 4 * 4) + j * (4 * 4);
    //         m2d[i][j]=Matrix<float>(4, 4, m);
    //     }
    // }

    // ConvBNLayer cbl(padding, stride, kernel_size, 3, 5, weight, bias);
    // // ConvBNLayer cbl(conv_params[0]);
    // // cbl.weight_m2d.print();
    // m2d = cbl.conv_bn_m2d(m2d);
    // // m2d.print();

    // ReLULayer rl;
    // m2d = rl.relu_m2d(m2d);
    // // m2d.print();

    // MaxPoolingLayer mpl(2, 2);
    // m2d = mpl.max_pooling_m2d(m2d);
    // // m2d.print();

    // float fc_weight[2 * 20];
    // for (int i = 0; i < 2 * 20; i++) {
    //     fc_weight[i] = (float)i;
    // }
    // float fc_bias[2] = {-1, -1};

    // FCLayer fcl(20, 2, fc_weight, fc_bias);
    // m2d = fcl.fully_connect_m2d(m2d);
    // // m2d[0][0].print();
    // m2d.print();  // [6439, 6439]

    // // SoftmaxLayer sml;
    // // m2d = sml.softmax_m2d(m2d);
    // // m2d.print();

    // // cv::Mat img = cv::imread("../samples/face.jpg");
    // // cv::Mat fimg;
    // // img.convertTo(fimg, CV_32FC3);
    // // fimg = fimg / 255;
    // // cout << fimg.channels() << endl;
    // // cout << fimg.size << endl;
    // cout << fimg << endl;

    return 0;
}
