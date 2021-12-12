#ifndef CNN_HPP
#define CNN_HPP

#include <iostream>

#include "params.hpp"
#include "matrix_2d_array.hpp"

using std::vector;

class Layer {
   public:
    virtual ~Layer() = default;
    virtual Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) = 0;
};

class ConvBNLayer : public Layer {
   private:
    bool pad;
    int stride;
    int kernel_size;
    int in_channels;
    int out_channels;
    Matrix2dArray<float> weight_m2d;
    float* bias;

   public:
    ConvBNLayer(int pad, int stride, int kernel_size, int in_channels, int out_channels, float* weight, float* bias);
    ConvBNLayer(conv_param& param);
    ConvBNLayer() = delete;

    Matrix2dArray<float> conv_bn(Matrix2dArray<float>& m2d);

    Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) override;
};

class ReLULayer : public Layer {
   public:
    Matrix2dArray<float> relu(Matrix2dArray<float>& m2d);

    Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) override;
};

class MaxPoolingLayer : public Layer {
   private:
    int size;

   public:
    MaxPoolingLayer(int size);
    MaxPoolingLayer() = delete;

    Matrix2dArray<float> max_pooling(Matrix2dArray<float>& m2d);

    Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) override;
};

class FCLayer : public Layer {
   private:
    int in_features;
    int out_features;
    Matrix2dArray<float> weight_m2d;
    float* bias;

   public:
    FCLayer(int in_features, int out_features, float* weight, float* bias);
    FCLayer(fc_param& param);
    FCLayer() = delete;

    Matrix2dArray<float> full_connect(Matrix2dArray<float>& m2d);

    Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) override;
};

class SoftmaxLayer : public Layer {
   public:
    Matrix2dArray<float> softmax(Matrix2dArray<float>& m2d);

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

inline ConvBNLayer::ConvBNLayer(int pad, int stride, int kernel_size, int in_channels, int out_channels, float* weight, float* bias) {
    this->pad = pad;
    this->stride = stride;
    this->kernel_size = kernel_size;
    this->in_channels = in_channels;
    this->out_channels = out_channels;
    this->weight_m2d = Matrix2dArray<float>(in_channels, out_channels, kernel_size, kernel_size, weight);
    this->bias = bias;
}

inline ConvBNLayer::ConvBNLayer(conv_param& param) {
    this->pad = param.pad;
    this->stride = param.stride;
    this->kernel_size = param.kernel_size;
    this->in_channels = param.in_channels;
    this->out_channels = param.out_channels;
    this->weight_m2d = Matrix2dArray<float>(param.in_channels, param.out_channels, param.kernel_size, param.kernel_size, param.p_weight);
    this->bias = param.p_bias;
}

Matrix2dArray<float> ConvBNLayer::conv_bn(Matrix2dArray<float>& m2d) {
    Matrix2dArray<float> res(1, this->out_channels, m2d.nrows, m2d.ncols);

    for (int i = 0; i < this->in_channels; i++) {
        Matrix<float> in_mat = m2d(0, i);
        for (int o = 0; o < this->out_channels; o++) {
            Matrix<float> weight_mat = this->weight_m2d(o, i);

            Matrix<float> conv_res = in_mat.convolution(weight_mat, this->pad, this->stride);
            res(0, o) += conv_res;
            // TODO conv_bn
        }
    }

    return res;
}

Matrix2dArray<float> ConvBNLayer::forward(Matrix2dArray<float>& m2d) {
    return this->conv_bn(m2d);
}

inline MaxPoolingLayer::MaxPoolingLayer(int size) {
    this->size = size;
}

inline FCLayer::FCLayer(int in_features, int out_features, float* weight, float* bias) {
    this->in_features = in_features;
    this->out_features = out_features;
    this->weight_m2d = Matrix2dArray<float>(1, 1, in_features, out_features, weight);
    this->bias = bias;
}

inline FCLayer::FCLayer(fc_param& param) {
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
