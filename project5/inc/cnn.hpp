#ifndef CNN_HPP
#define CNN_HPP

#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#include "matrix.hpp"
#include "params.hpp"

using std::vector;

#define IMAGE_SIZE_128 128

typedef Matrix<Matrix<float>> M2D;

void print_m2d(M2D& m2d) {
    for (int d1 = 0; d1 < m2d.nrows; d1++) {
        for (int d2 = 0; d2 < m2d.ncols; d2++) {
            cout << "Row " << d1 << " Col " << d2 << endl;
            m2d[d1][d2].print();
            cout << endl;
        }
    }
}

Matrix<float> flatten(M2D& m2d) {
    Matrix<float> res(1, m2d.nrows * m2d.ncols * m2d[0][0].nrows * m2d[0][0].ncols);

    float* curr_p = res.data;
    for (int d1 = 0; d1 < m2d.nrows; d1++) {
        for (int d2 = 0; d2 < m2d.ncols; d2++) {
            memcpy(curr_p, m2d[d1][d2].data, m2d[d1][d2].nrows * m2d[d1][d2].ncols * sizeof(float));
            curr_p += m2d[d1][d2].nrows * m2d[d1][d2].ncols;
        }
    }

    return res;
}

class Layer {
   public:
    virtual ~Layer() = default;
    virtual M2D forward(M2D& m2d) = 0;
};

class ConvBNLayer : public Layer {
   private:
    bool padding;
    int stride;
    int kernel_size;
    int in_channels;
    int out_channels;
    M2D weight_m2d;
    float* bias;

   public:
    ConvBNLayer(int padding, int stride, int kernel_size, int in_channels, int out_channels, float* weight, float* bias);
    ConvBNLayer(conv_param& param);
    ConvBNLayer() = delete;

    M2D conv_bn_m2d(M2D& m2d);

    M2D forward(M2D& m2d) override;
};

class ReLULayer : public Layer {
   public:
    M2D relu_m2d(M2D& m2d);

    M2D forward(M2D& m2d) override;
};

class MaxPoolingLayer : public Layer {
   private:
    int pool_size;
    int stride;

   public:
    MaxPoolingLayer(int pool_size, int stride);
    MaxPoolingLayer() = delete;

    M2D max_pooling_m2d(M2D& m2d);

    M2D forward(M2D& m2d) override;
};

class FCLayer : public Layer {
   private:
    int in_features;
    int out_features;
    M2D weight_m2d;
    float* bias;

   public:
    FCLayer(int in_features, int out_features, float* weight, float* bias);
    FCLayer(fc_param& param);
    FCLayer() = delete;

    M2D fully_connect_m2d(M2D& m2d);

    M2D forward(M2D& m2d) override;
};

class SoftmaxLayer : public Layer {
   public:
    M2D softmax_m2d(M2D& m2d);

    M2D forward(M2D& m2d) override;
};

class CNN {
   private:
    vector<Layer*> layers;

   public:
    void add_layer(Layer* l);

    M2D load_image(const char* image_path, int image_size);
    M2D predict(const char* image_path, int image_size);

    static void face_detection(const char* image_path, bool p_flag = false);
};

// -------------------------------------------------------------------------------------------

using std::cin;
using std::cout;
using std::endl;

ConvBNLayer::ConvBNLayer(int padding, int stride, int kernel_size, int in_channels, int out_channels, float* weight, float* bias) {
    this->padding = padding;
    this->stride = stride;
    this->kernel_size = kernel_size;
    this->in_channels = in_channels;
    this->out_channels = out_channels;

    this->weight_m2d = M2D(out_channels, in_channels);
    for (int o = 0; o < out_channels; o++) {
        for (int i = 0; i < in_channels; i++) {
            float* kernel_oi = weight + o * (in_channels * kernel_size * kernel_size) + i * (kernel_size * kernel_size);
            this->weight_m2d[o][i] = Matrix<float>(kernel_size, kernel_size, kernel_oi);
        }
    }

    this->bias = bias;
}

ConvBNLayer::ConvBNLayer(conv_param& param) {
    this->padding = param.pad;
    this->stride = param.stride;
    this->kernel_size = param.kernel_size;
    this->in_channels = param.in_channels;
    this->out_channels = param.out_channels;

    this->weight_m2d = M2D(param.out_channels, param.in_channels);
    for (int o = 0; o < param.out_channels; o++) {
        for (int i = 0; i < param.in_channels; i++) {
            float* kernel_oi = param.p_weight + o * (param.in_channels * param.kernel_size * param.kernel_size) + i * (param.kernel_size * param.kernel_size);
            this->weight_m2d[o][i] = Matrix<float>(param.kernel_size, param.kernel_size, kernel_oi);
        }
    }

    this->bias = param.p_bias;
}

M2D ConvBNLayer::conv_bn_m2d(M2D& m2d) {
    M2D res(1, this->out_channels);
    for (int j = 0; j < this->out_channels; j++) {
        res[0][j] = Matrix<float>((m2d[0][0].nrows + 2 * padding - kernel_size) / stride + 1, (m2d[0][0].nrows + 2 * padding - kernel_size) / stride + 1, 0.0f);
    }

    for (int i = 0; i < this->in_channels; i++) {
        Matrix<float> in_mat = m2d(0, i);
        for (int o = 0; o < this->out_channels; o++) {
            Matrix<float> weight_mat = this->weight_m2d(o, i);

            Matrix<float> conv_res = in_mat.convolution(weight_mat, this->padding, this->stride);
            res(0, o) += conv_res;
        }
    }

    for (int o = 0; o < out_channels; o++) {
        res(0, o) += this->bias[o];
    }

#ifndef NDBUG
    cout << "conv res size = "
         << "[" << res.nrows << ", " << res.ncols << "] " << res[0][0].nrows << " * " << res[0][0].ncols << endl;
#endif

    return res;
}

M2D ConvBNLayer::forward(M2D& m2d) {
    return this->conv_bn_m2d(m2d);
}

M2D ReLULayer::relu_m2d(M2D& m2d) {
    M2D res = M2D(m2d.nrows, m2d.ncols);

    for (int i = 0; i < m2d.nrows; i++) {
        for (int j = 0; j < m2d.ncols; j++) {
            Matrix<float> in_mat = m2d(i, j);

            Matrix<float> pooling_res = in_mat.relu();
            res(i, j) = pooling_res;
        }
    }

#ifndef NDBUG
    cout << "relu res size = "
         << "[" << res.nrows << ", " << res.ncols << "] " << res[0][0].nrows << " * " << res[0][0].ncols << endl;
#endif

    return res;
}

M2D ReLULayer::forward(M2D& m2d) {
    return this->relu_m2d(m2d);
}

inline MaxPoolingLayer::MaxPoolingLayer(int pool_size, int stride) {
    this->pool_size = pool_size;
    this->stride = stride;
}

M2D MaxPoolingLayer::max_pooling_m2d(M2D& m2d) {
    M2D res(m2d.nrows, m2d.ncols);

    for (int i = 0; i < m2d.nrows; i++) {
        for (int j = 0; j < m2d.ncols; j++) {
            Matrix<float> in_mat = m2d(i, j);

            Matrix<float> pooling_res = in_mat.max_pooling(pool_size, stride);
            res(i, j) = pooling_res;
        }
    }

#ifndef NDBUG
    cout << "max pooling res size = "
         << "[" << res.nrows << ", " << res.ncols << "] " << res[0][0].nrows << " * " << res[0][0].ncols << endl;
#endif

    return res;
}

M2D MaxPoolingLayer::forward(M2D& m2d) {
    return this->max_pooling_m2d(m2d);
}

inline FCLayer::FCLayer(int in_features, int out_features, float* weight, float* bias) {
    this->in_features = in_features;
    this->out_features = out_features;

    this->weight_m2d = M2D(1, 1);
    this->weight_m2d[0][0] = Matrix<float>(out_features, in_features, weight);

    this->bias = bias;
}

inline FCLayer::FCLayer(fc_param& param) {
    this->in_features = param.in_features;
    this->out_features = param.out_features;

    this->weight_m2d = M2D(1, 1);
    this->weight_m2d[0][0] = Matrix<float>(param.out_features, param.in_features, param.p_weight);

    this->bias = param.p_bias;
}

M2D FCLayer::fully_connect_m2d(M2D& m2d) {
    Matrix<float> weight_t = this->weight_m2d[0][0].transpose();
    Matrix<float> flattened_matrix = flatten(m2d);
    Matrix<float> product = flattened_matrix * weight_t;

    assert(product.get_nrows() == 1);
    assert(product.get_ncols() == this->out_features);

    for (int j = 0; j < product.get_ncols(); j++) {
        product[0][j] += bias[j];
    }

    M2D res(1, 1);
    res[0][0] = product;

#ifndef NDBUG
    cout << "fully connect res size = "
         << "[" << res.nrows << ", " << res.ncols << "] " << res[0][0].nrows << " * " << res[0][0].ncols << endl;
#endif

    return res;
}

M2D FCLayer::forward(M2D& m2d) {
    return this->fully_connect_m2d(m2d);
}

M2D SoftmaxLayer::softmax_m2d(M2D& m2d) {
    assert(m2d.nrows == 1 && m2d.ncols == 1 && m2d[0][0].nrows == 1);

    float sum_exp = 0;
    for (int i = 0; i < m2d[0][0].nrows * m2d[0][0].ncols; i++) {
        sum_exp += std::exp(m2d[0][0].data[i]);
    }

    for (int i = 0; i < m2d[0][0].nrows * m2d[0][0].ncols; i++) {
        m2d[0][0].data[i] = std::exp(m2d[0][0].data[i]) / sum_exp;
    }

    return m2d;
}

M2D SoftmaxLayer::forward(M2D& m2d) {
    return this->softmax_m2d(m2d);
}

inline void CNN::add_layer(Layer* l) {
    this->layers.push_back(l);
}

M2D CNN::load_image(const char* image_path, int image_size) {
    cv::Mat cv_image = cv::imread(image_path);

    if (cv_image.empty()) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Loading image error: the input path is not an image." << endl;
        exit(EXIT_FAILURE);
    }

    if (cv_image.rows != image_size || cv_image.cols != image_size) {
        cv::resize(cv_image, cv_image, cv::Size(image_size, image_size));
    }

    int nrows = cv_image.rows;
    int ncols = cv_image.cols;

    M2D res(1, 3);
    for (int j = 0; j < 3; j++) {
        res[0][j] = Matrix<float>(nrows, ncols, 0.0f);
    }

    uchar* pdata = cv_image.data;
    for (int i = 0; i < nrows * ncols; i++) {
        res[0][0].data[i] = (float)pdata[2] / 255.0f;  // R
        res[0][1].data[i] = (float)pdata[1] / 255.0f;  // G
        res[0][2].data[i] = (float)pdata[0] / 255.0f;  // B
        pdata += 3;
    }

    return res;
}

inline M2D CNN::predict(const char* image_path, int image_size) {
    M2D res = load_image(image_path, image_size);
    for (auto l : this->layers) {
        res = l->forward(res);
    }
    return res;
}

void CNN::face_detection(const char* image_path, bool p_flag) {
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

    M2D res = cnn.predict(image_path, IMAGE_SIZE_128);

    assert(res.nrows == res.ncols == 1);
    assert(res[0][0].nrows == 1 && res[0][0].ncols == 2);

    float bg = res[0][0].data[0];
    float face = res[0][0].data[1];

    if (p_flag) {
        cout << std::setiosflags(std::ios::fixed) << std::setprecision(2) << "The probability of background is: " << bg * 100 << "%" << endl;
        cout << std::setiosflags(std::ios::fixed) << std::setprecision(2) << "The probability of face is: " << face * 100 << "%" << endl;
    } else {
        cout << "The probability of background is: " << bg << endl;
        cout << "The probability of face is: " << face << endl;
    }

    if (bg > face) {
        cout << "This image is background." << endl;
    } else {
        cout << "This image is face." << endl;
    }
}

#endif
