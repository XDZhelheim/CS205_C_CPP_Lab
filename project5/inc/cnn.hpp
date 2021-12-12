#ifndef CNN_HPP
#define CNN_HPP

#include <iostream>
#include <opencv2/opencv.hpp>

#include "matrix.hpp"
#include "params.hpp"

using std::vector;

template <typename T>
class Matrix2dArray {
   public:
    int dim1;
    int dim2;
    int nrows;
    int ncols;
    Matrix<T> base_mat;

   public:
    Matrix2dArray(int dim1, int dim2, int nrows, int ncols, T* data);
    Matrix2dArray(int dim1, int dim2, int nrows, int ncols);
    Matrix2dArray(const cv::Mat& cv_image);
    Matrix2dArray(const char* image_path);
    Matrix2dArray() = default;
    Matrix2dArray(const Matrix2dArray<T>& other) = default;
    ~Matrix2dArray() = default;

    Matrix<T> operator()(int d1, int d2);
    T& operator()(int d1, int d2, int i, int j);

    Matrix2dArray<T>& operator=(const Matrix2dArray<T>& other);
    Matrix2dArray<T> read_image(const char* image_path);
    void print();
};
class Layer {
   public:
    virtual ~Layer() = default;
    virtual Matrix2dArray<float> forward(Matrix2dArray<float>& m2d) = 0;
};

class ConvBNLayer : public Layer {
   private:
    int pad;
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

#endif
