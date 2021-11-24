#pragma once

#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

#define STRASSEN_LOWER_BOUND 128

template <typename T>
class matrix {
   public:
    // fields
    int nrows;
    int ncols;
    T* data;

    const matrix<T>* parent_matrix;
    int* ref_count;

    int* shape();
    void print_shape();
    bool shape_equals(matrix<T>& other);

    // construction & destruction
    matrix();
    matrix(int nrows, int ncols);
    matrix(int nrows, int ncols, T fill);
    matrix(const matrix<T>& other);
    ~matrix();

    matrix<T> copy();

    matrix<T>& operator=(const matrix<T>& other);

    static matrix<T> create_row_vec(int ncols, T fill);
    static matrix<T> create_col_vec(int nrows, T fill);

    static matrix<T> create_diagonal(int nrows, T fill);

    // I/O
    static matrix<T> read_matrix(const char* file_name);
    void print(const char* file_name);
    void print();

    // operations
    T* operator[](int i);
    T& operator()(int i, int j);
    matrix<T> operator+(matrix<T>& other);
    matrix<T> operator-(matrix<T>& other);
    matrix<T> operator*(matrix<T>& other);
    matrix<T> operator*(T coef);
    template <typename U> friend matrix<U> operator*(int coef, matrix<U>& m);
    matrix<T> operator^(int expo);
    matrix<T>& operator*=(matrix<T>& other);
    matrix<T>& operator*=(T coef);
    matrix<T>& operator+=(matrix<T>& other);
    matrix<T>& operator-=(matrix<T>& other);

    matrix<T> multiply_elements(matrix<T>& other);
    static matrix<T> multiply_matrix(matrix<T>& m1, matrix<T>& m2);
    static matrix<T> strassen(matrix<T>& A, matrix<T>& B);

    // submatrix and merge matrix
    matrix<T> submatrix_ROI(int row_start, int row_end, int col_start, int col_end);
    matrix<T> submatrix_cpy(int row_start, int row_end, int col_start, int col_end);
    matrix<T> submatrix(int row_start, int row_end, int col_start, int col_end);
    void adjust_ROI(int row_start, int row_end, int col_start, int col_end);
    static matrix<T> merge_matrix(matrix<T>& C11, matrix<T>& C12, matrix<T>& C21, matrix<T>& C22);
};
