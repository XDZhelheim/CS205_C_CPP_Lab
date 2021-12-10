#ifndef CNN_HPP
#define CNN_HPP

#include <iostream>
#include <opencv2/opencv.hpp>

#include "params.hpp"

using cv::Mat;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

class layer {
   public:
    virtual Mat forward(Mat& mat) = 0;
};

class conv_bn_layer : public layer {
   private:
    int pad;
    int stride;
    int kernel_size;
    int in_channels;
    int out_channels;
    float* weight;
    float* bias;

   public:
    conv_bn_layer(int pad, int stride, int kernel_size, int in_channels, int out_channels, float* weight, float* bias) {
        this->pad = pad;
        this->stride = stride;
        this->kernel_size = kernel_size;
        this->in_channels = in_channels;
        this->out_channels = out_channels;
        this->weight = weight;
        this->bias = bias;
    }

    conv_bn_layer(conv_param& param) {
        this->pad = param.pad;
        this->stride = param.stride;
        this->kernel_size = param.kernel_size;
        this->in_channels = param.in_channels;
        this->out_channels = param.out_channels;
        this->weight = param.p_weight;
        this->bias = param.p_bias;
    }

    conv_bn_layer() = delete;
    ~conv_bn_layer() = default;

    Mat conv_bn(Mat& mat);

    Mat forward(Mat& mat) override;
};

class relu_layer : public layer {
   public:
    Mat relu(Mat& mat);

    Mat forward(Mat& mat) override;
};

class max_pooling_layer : public layer {
   public:
    int size;

    max_pooling_layer(int size) {
        this->size = size;
    }

    max_pooling_layer() = delete;

    Mat max_pooling(Mat& mat);

    Mat forward(Mat& mat) override;
};

class fc_layer : public layer {
   public:
    int in_size;
    int out_size;

    fc_layer(int in_size, int out_size) {
        this->in_size = in_size;
        this->out_size = out_size;
    }

    fc_layer() = delete;

    Mat full_connect(Mat& mat);

    Mat forward(Mat& mat) override;
};

class cnn {
   public:
    vector<layer*> layers;

    void add_layer(layer* l) {
        layers.push_back(l);
    }

    Mat prediction(Mat& image) {
        Mat res = image;
        for (auto l : layers) {
            res = l->forward(res);
        }
        return res;
    }
};

#endif