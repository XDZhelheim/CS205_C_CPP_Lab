#pragma once

#include <iostream>

using namespace std;

#define STRASSEN_LOWER_BOUND 128
#define NAN -1

template <typename T>
class matrix {
private:
    int nrows;
    int ncols;
    T* data;

public:
    matrix(int nrows, int ncols, T fill);
    ~matrix();

    T* operator[](int i);
    matrix<T> operator+(const matrix& other);
    matrix<T> operator-(const matrix& other);
    matrix<T> operator*(const matrix& other);

    matrix<T> read_matrix(const char* file_name);
    void print(const char* file_name);

    matrix<T> copy();
    matrix<T> submatrix(int row_start, int row_end, int col_start, int col_end);

    static matrix<T> merge_matrix(const matrix& C11, const matrix& C12, const matrix& C21, const matrix& C22);

};