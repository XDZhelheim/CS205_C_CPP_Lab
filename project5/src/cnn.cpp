#include "cnn.hpp"

#include "matrix.cpp"

using std::cin;
using std::cout;
using std::endl;

template <typename T>
inline Matrix2dArray<T>::Matrix2dArray(int dim1, int dim2, int nrows, int ncols, T* data) {
    this->dim1 = dim1;
    this->dim2 = dim2;
    this->nrows = nrows;
    this->ncols = ncols;
    this->base_mat = Matrix<T>(dim1 * nrows, dim2 * ncols, data);
}

template <typename T>
inline Matrix2dArray<T>::Matrix2dArray(int dim1, int dim2, int nrows, int ncols) {
    this->dim1 = dim1;
    this->dim2 = dim2;
    this->nrows = nrows;
    this->ncols = ncols;
    this->base_mat = Matrix<T>(dim1 * nrows, dim2 * ncols);
}

template <typename T>
inline Matrix2dArray<T>::Matrix2dArray(const cv::Mat& cv_image) {
    // TODO
}

template <typename T>
inline Matrix2dArray<T>::Matrix2dArray(const char* image_path) {
    cv::Mat cv_image = cv::imread(image_path);

    *this = Matrix2dArray<T>(cv_image);
}

template <typename T>
inline Matrix<T> Matrix2dArray<T>::operator()(int d1, int d2) {
    if (d1 > this->dim1 - 1 || d2 > this->dim2 - 1) {
        cout << "Index error: array index out of bound." << endl;
        exit(EXIT_FAILURE);
    }

    return this->base_mat.submatrix_ROI(d1 * this->nrows, d1 * this->nrows + this->nrows, d2 * this->ncols, d2 * this->ncols + this->ncols);
}

template <typename T>
inline T& Matrix2dArray<T>::operator()(int d1, int d2, int i, int j) {
    if (d1 > this->dim1 - 1 || d2 > this->dim2 - 1) {
        cout << "Index error: array index out of bound." << endl;
        exit(EXIT_FAILURE);
    }

    if (i > this->nrows - 1 || j > this->ncols - 1) {
        cout << "Index error: array index out of bound." << endl;
        exit(EXIT_FAILURE);
    }

    return this->operator()(d1, d2).operator()(i, j);
}

template <typename T>
inline Matrix2dArray<T>& Matrix2dArray<T>::operator=(const Matrix2dArray<T>& other) {
    this->dim1 = other.dim1;
    this->dim2 = other.dim2;
    this->nrows = other.nrows;
    this->ncols = other.ncols;
    this->base_mat = other.base_mat;

    return *this;
}

template <typename T>
Matrix2dArray<T> Matrix2dArray<T>::read_image(const char* image_path) {
    return Matrix2dArray(image_path);
}

template <typename T>
void Matrix2dArray<T>::print() {
    for (int i = 0; i < this->dim1; i++) {
        for (int j = 0; i < this->dim2; j++) {
            this->operator()(i, j).print();
        }
        cout << endl;
    }
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