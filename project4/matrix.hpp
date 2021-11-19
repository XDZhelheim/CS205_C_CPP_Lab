#pragma once

#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

#define STRASSEN_LOWER_BOUND 1024
#define NAN -1

template <typename T>
class matrix {
   public:
    // fields
    int nrows;
    int ncols;
    T* data;

    const matrix<T>* parent_matrix;

    int* shape();
    void print_shape();

    // construction & destruction
    matrix();
    matrix(int nrows, int ncols);
    matrix(int nrows, int ncols, T fill);
    matrix(const matrix<T>& other);
    ~matrix();

    matrix<T> copy();

    matrix<T>& operator=(matrix<T>& other);

    matrix<T> create_row_vec(int ncols, T fill);
    matrix<T> create_col_vec(int nrows, T fill);

    // I/O
    template <typename U> friend ostream& operator<<(ostream& out, const matrix<U>& m);  // mainly used to print matrix on cout
    static matrix<T> read_matrix(const char* file_name);
    void print(const char* file_name);

    // operations
    T* operator[](int i);
    T& operator()(int i, int j);
    matrix<T> operator+(matrix<T>& other);
    matrix<T> operator-(matrix<T>& other);
    matrix<T> operator*(matrix<T>& other);

    matrix<T> multiply_elements(matrix<T>& other);

    // submatrix and merge matrix
    matrix<T> submatrix_ROI(int row_start, int row_end, int col_start, int col_end);
    matrix<T> submatrix_cpy(int row_start, int row_end, int col_start, int col_end);
    matrix<T> submatrix(int row_start, int row_end, int col_start, int col_end);
    void adjust_ROI(int row_start, int row_end, int col_start, int col_end);
    static matrix<T> merge_matrix(matrix<T>& C11, matrix<T>& C12, matrix<T>& C21, matrix<T>& C22);
};
