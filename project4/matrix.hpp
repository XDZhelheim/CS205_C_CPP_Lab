#pragma once

#include <ctime>
#include <iostream>
#include <fstream>

using namespace std;

#define STRASSEN_LOWER_BOUND 1024
#define NAN -1

template <typename T>
class matrix {
   public:
    int nrows;
    int ncols;
    T* data;

    matrix(const matrix<T>& other);

    matrix(int nrows, int ncols, T fill);
    ~matrix();

    T* operator[](int i);
    T& operator()(int i, int j);
    matrix<T> operator+(matrix<T>& other);
    matrix<T> operator-(matrix<T>& other);
    matrix<T> operator*(matrix<T>& other);

    static matrix<T> read_matrix(const char* file_name);
    void print(const char* file_name);

    matrix<T> submatrix(int row_start, int row_end, int col_start, int col_end);
    static matrix<T> merge_matrix(matrix<T>& C11, matrix<T>& C12, matrix<T>& C21, matrix<T>& C22);

    matrix<T> copy();
};
