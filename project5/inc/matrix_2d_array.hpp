#ifndef MATRIX_2D_ARRAY_HPP
#define MATRIX_2D_ARRAY_HPP

#include <opencv2/opencv.hpp>
#include "matrix.hpp"

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

// --------------------------------------------------------------------------------------------------------------------

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
    this->base_mat = Matrix<T>(dim1 * nrows, dim2 * ncols, (T)0);
}

template <typename T>
inline Matrix2dArray<T>::Matrix2dArray(const cv::Mat& cv_image) {
    // TODO convert cv::Mat
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

#endif