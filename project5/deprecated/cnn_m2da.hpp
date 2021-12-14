#ifndef CNN_M2DA_HPP
#define CNN_M2DA_HPP

#include <cmath>
#include <iostream>
#include <vector>

#include "matrix_2d_array.hpp"
#include "params.hpp"

using std::vector;

class Layer {
   public:
    virtual ~Layer() = default;
    virtual Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) = 0;
};

class ConvBNLayer : public Layer {
   public:
    bool padding;
    int stride;
    int kernel_size;
    int in_channels;
    int out_channels;
    Matrix2dArray<float> weight_m2d;
    float* bias;

   public:
    ConvBNLayer(int padding, int stride, int kernel_size, int in_channels, int out_channels, float* weight, float* bias);
    ConvBNLayer(conv_param& param);
    ConvBNLayer() = delete;

    Matrix2dArray<float> conv_bn_m2d(Matrix2dArray<float>& m2d);

    Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) override;
};

class ReLULayer : public Layer {
   public:
    Matrix2dArray<float> relu_m2d(Matrix2dArray<float>& m2d);

    Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) override;
};

class MaxPoolingLayer : public Layer {
   public:
    int pool_size;
    int stride;

   public:
    MaxPoolingLayer(int pool_size, int stride);
    MaxPoolingLayer() = delete;

    Matrix2dArray<float> max_pooling_m2d(Matrix2dArray<float>& m2d);

    Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) override;
};

class FCLayer : public Layer {
   public:
    int in_features;
    int out_features;
    Matrix2dArray<float> weight_m2d;
    float* bias;

   public:
    FCLayer(int in_features, int out_features, float* weight, float* bias);
    FCLayer(fc_param& param);
    FCLayer() = delete;

    Matrix2dArray<float> fully_connect_m2d(Matrix2dArray<float>& m2d);

    Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) override;
};

class SoftmaxLayer : public Layer {
   public:
    Matrix2dArray<float> softmax_m2d(Matrix2dArray<float>& m2d);

    Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) override;
};

class CNN {
   public:
    vector<Layer*> layers;

    void add_layer(Layer* l);

    Matrix2dArray<float> predict(const char* image_path);
};

// -------------------------------------------------------------------------------------------

using std::cin;
using std::cout;
using std::endl;

inline ConvBNLayer::ConvBNLayer(int padding, int stride, int kernel_size, int in_channels, int out_channels, float* weight, float* bias) {
    this->padding = padding;
    this->stride = stride;
    this->kernel_size = kernel_size;
    this->in_channels = in_channels;
    this->out_channels = out_channels;
    float* converted_array = Matrix2dArray<float>::convert_data_array(out_channels, in_channels, kernel_size, kernel_size, weight);
    this->weight_m2d = Matrix2dArray<float>(out_channels, in_channels, kernel_size, kernel_size, converted_array);
    delete[] converted_array;
    this->bias = bias;
}

inline ConvBNLayer::ConvBNLayer(conv_param& param) {
    this->padding = param.pad;
    this->stride = param.stride;
    this->kernel_size = param.kernel_size;
    this->in_channels = param.in_channels;
    this->out_channels = param.out_channels;
    float* converted_array = Matrix2dArray<float>::convert_data_array(param.out_channels, param.in_channels, param.kernel_size, param.kernel_size, param.p_weight);
    this->weight_m2d = Matrix2dArray<float>(param.out_channels, param.in_channels, param.kernel_size, param.kernel_size, converted_array);
    delete[] converted_array;
    this->bias = param.p_bias;
}

Matrix2dArray<float> ConvBNLayer::conv_bn_m2d(Matrix2dArray<float>& m2d) {
    Matrix2dArray<float> res(1, this->out_channels, (m2d.nrows + 2 * padding - kernel_size) / stride + 1, (m2d.ncols + 2 * padding - kernel_size) / stride + 1);

    cout << "conv res size = "
         << "[" << res.dim1 << ", " << res.dim2 << "] " << res.nrows << " * " << res.ncols << endl;

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

    return res;
}

Matrix2dArray<float> ConvBNLayer::forward(Matrix2dArray<float>& m2d) {
    return this->conv_bn_m2d(m2d);
}

Matrix2dArray<float> ReLULayer::relu_m2d(Matrix2dArray<float>& m2d) {
    Matrix2dArray<float> res(m2d.dim1, m2d.dim2, m2d.nrows, m2d.ncols);

    cout << "relu res size = "
         << "[" << res.dim1 << ", " << res.dim2 << "] " << res.nrows << " * " << res.ncols << endl;

    for (int i = 0; i < m2d.dim1; i++) {
        for (int j = 0; j < m2d.dim2; j++) {
            Matrix<float> in_mat = m2d(i, j);

            Matrix<float> pooling_res = in_mat.relu();
            res(i, j) += pooling_res;
        }
    }

    return res;
}

Matrix2dArray<float> ReLULayer::forward(Matrix2dArray<float>& m2d) {
    return this->relu_m2d(m2d);
}

inline MaxPoolingLayer::MaxPoolingLayer(int pool_size, int stride) {
    this->pool_size = pool_size;
    this->stride = stride;
}

Matrix2dArray<float> MaxPoolingLayer::max_pooling_m2d(Matrix2dArray<float>& m2d) {
    Matrix2dArray<float> res(m2d.dim1, m2d.dim2, (m2d.nrows - pool_size) / stride + 1, (m2d.ncols - pool_size) / stride + 1);

    cout << "max pooling res size = "
         << "[" << res.dim1 << ", " << res.dim2 << "] " << res.nrows << " * " << res.ncols << endl;

    for (int i = 0; i < m2d.dim1; i++) {
        for (int j = 0; j < m2d.dim2; j++) {
            Matrix<float> in_mat = m2d(i, j);

            Matrix<float> pooling_res = in_mat.max_pooling(pool_size, stride);
            res(i, j) += pooling_res;
        }
    }

    return res;
}

Matrix2dArray<float> MaxPoolingLayer::forward(Matrix2dArray<float>& m2d) {
    return this->max_pooling_m2d(m2d);
}

inline FCLayer::FCLayer(int in_features, int out_features, float* weight, float* bias) {
    this->in_features = in_features;
    this->out_features = out_features;
    float* converted_array = Matrix2dArray<float>::convert_data_array(1, 1, out_features, in_features, weight);
    this->weight_m2d = Matrix2dArray<float>(1, 1, out_features, in_features, converted_array);
    delete[] converted_array;
    this->bias = bias;
}

inline FCLayer::FCLayer(fc_param& param) {
    this->in_features = param.in_features;
    this->out_features = param.out_features;
    float* converted_array = Matrix2dArray<float>::convert_data_array(1, 1, param.out_features, param.in_features, param.p_weight);
    this->weight_m2d = Matrix2dArray<float>(1, 1, param.out_features, param.in_features, converted_array);
    delete[] converted_array;
    this->bias = param.p_bias;
}

Matrix2dArray<float> FCLayer::fully_connect_m2d(Matrix2dArray<float>& m2d) {
    Matrix<float> weight_t = this->weight_m2d.base_mat.transpose();
    Matrix<float> flattened_matrix = Matrix<float>(1, m2d.base_mat.get_nrows() * m2d.base_mat.get_ncols(), Matrix2dArray<float>::reverse_convert_data_array(m2d.dim1, m2d.dim2, m2d.nrows, m2d.ncols, m2d.base_mat.get_data()));
    Matrix<float> product = flattened_matrix * weight_t;

    assert(product.get_nrows() == 1);
    assert(product.get_ncols() == this->out_features);

    for (int j = 0; j < product.get_ncols(); j++) {
        product[0][j] += bias[j];
    }

    Matrix2dArray<float> res(1, 1, product.get_nrows(), product.get_ncols(), product);

    cout << "fully connect res size = "
         << "[" << res.dim1 << ", " << res.dim2 << "] " << res.nrows << " * " << res.ncols << endl;

    return res;
}

Matrix2dArray<float> FCLayer::forward(Matrix2dArray<float>& m2d) {
    return this->fully_connect_m2d(m2d);
}

Matrix2dArray<float> SoftmaxLayer::softmax_m2d(Matrix2dArray<float>& m2d) {
    assert(m2d.dim1 == 1 && m2d.nrows == 1);

    float sum_exp = 0;
    for (int i = 0; i < m2d.nrows * m2d.ncols; i++) {
        sum_exp += std::exp(m2d.base_mat.get_data()[i]);
    }
    for (int i = 0; i < m2d.nrows * m2d.ncols; i++) {
        m2d.base_mat.get_data()[i] = std::exp(m2d.base_mat.get_data()[i]) / sum_exp;
    }

    return m2d;
}

Matrix2dArray<float> SoftmaxLayer::forward(Matrix2dArray<float>& m2d) {
    return this->softmax_m2d(m2d);
}

inline void CNN::add_layer(Layer* l) {
    this->layers.push_back(l);
}

inline Matrix2dArray<float> CNN::predict(const char* image_path) {
    Matrix2dArray<float> res = image_path;
    for (auto l : this->layers) {
        res = l->forward(res);
    }
    return res;
}

#endif
