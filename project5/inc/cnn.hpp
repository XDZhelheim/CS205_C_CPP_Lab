#ifndef CNN_HPP
#define CNN_HPP

#include <iostream>
#include <opencv2/opencv.hpp>

#include "params.hpp"

using cv::imread;
using cv::Mat;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

class Layer {
   public:
    virtual ~Layer() = default;
    virtual Mat forward(Mat& mat) = 0;
};

class ConvBNLayer : public Layer {
   private:
    int pad;
    int stride;
    int kernel_size;
    int in_channels;
    int out_channels;
    float* weight;
    float* bias;

   public:
    ConvBNLayer(int pad, int stride, int kernel_size, int in_channels, int out_channels, float* weight, float* bias) {
        this->pad = pad;
        this->stride = stride;
        this->kernel_size = kernel_size;
        this->in_channels = in_channels;
        this->out_channels = out_channels;
        this->weight = weight;
        this->bias = bias;
    }

    ConvBNLayer(conv_param& param) {
        this->pad = param.pad;
        this->stride = param.stride;
        this->kernel_size = param.kernel_size;
        this->in_channels = param.in_channels;
        this->out_channels = param.out_channels;
        this->weight = param.p_weight;
        this->bias = param.p_bias;
    }

    ConvBNLayer() = delete;

    Mat conv_bn(Mat& mat);

    Mat forward(Mat& mat) override;
};

class ReLULayer : public Layer {
   public:
    Mat relu(Mat& mat);

    Mat forward(Mat& mat) override;
};

class MaxPoolingLayer : public Layer {
   public:
    int size;

    MaxPoolingLayer(int size) {
        this->size = size;
    }

    MaxPoolingLayer() = delete;

    Mat max_pooling(Mat& mat);

    Mat forward(Mat& mat) override;
};

class FCLayer : public Layer {
   public:
    int in_size;
    int out_size;

    FCLayer(int in_size, int out_size) {
        this->in_size = in_size;
        this->out_size = out_size;
    }

    FCLayer() = delete;

    Mat full_connect(Mat& mat);

    Mat forward(Mat& mat) override;
};

class CNN {
   public:
    vector<Layer*> layers;

    void add_layer(Layer* l);

    Mat predict(Mat& image);
};

#endif
