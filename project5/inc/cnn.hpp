#ifndef CNN_HPP
#define CNN_HPP

#include <iostream>
#include <opencv2/opencv.hpp>

#include "params.hpp"

using std::cin;
using std::cout;
using std::endl;
using cv::Mat;

class layer {
   public:
    virtual Mat forward(Mat mat) = 0;
};

class conv_bn_relu_layer : public layer {
   private:
    int pad;
    int stride;
    int kernel_size;
    int in_channels;
    int out_channels;
    float* weight;
    float* bias;

   public:
    conv_bn_relu_layer(int pad, int stride, int kernel_size, int in_channels, int out_channels, float* weight, float* bias) {
        this->pad = pad;
        this->stride = stride;
        this->kernel_size = kernel_size;
        this->in_channels = in_channels;
        this->out_channels = out_channels;
        this->weight = weight;
        this->bias = bias;
    }

    conv_bn_relu_layer(conv_param& param) {
        this->pad = param.pad;
        this->stride = param.stride;
        this->kernel_size = param.kernel_size;
        this->in_channels = param.in_channels;
        this->out_channels = param.out_channels;
        this->weight = param.p_weight;
        this->bias = param.p_bias;
    }

    conv_bn_relu_layer() = delete;
    ~conv_bn_relu_layer() = default;

    Mat conv_bn(Mat mat);
    Mat relu(Mat mat);

    Mat forward(Mat mat) override;
};

class fc_layer : public layer {

};

class cnn {
};

#endif