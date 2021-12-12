/**
 * @file matrix.cpp
 * @author Zheng Dong (11812804@mail.sustech.edu.cn) github.com/XDZhelheim
 * @brief Implementation of `Matrix` class
 * @version 1.0.1
 * @date 2021-12-11
 * 
 * @copyright Copyright (c) 2021 Zheng Dong
 * 
 */

#include "matrix.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

template <typename T>
inline int Matrix<T>::get_nrows() {
    return this->nrows;
}

template <typename T>
inline int Matrix<T>::get_ncols() {
    return this->ncols;
}

template <typename T>
inline T* Matrix<T>::get_data() {
    return this->data;
}

template <typename T>
inline int* Matrix<T>::shape() {
    return new int[2]{this->nrows, this->ncols};
}

template <typename T>
inline void Matrix<T>::print_shape() {
    cout << "(" << this->nrows << ", " << this->ncols << ")" << endl;
}

template <typename T>
inline bool Matrix<T>::shape_equals(Matrix<T>& other) {
    return this->nrows == other.nrows && this->ncols == other.ncols;
}

// ---------------------------------------------------------------------------------
template <typename T>
inline Matrix<T>::Matrix() {
    this->nrows = 0;
    this->ncols = 0;
    this->data = nullptr;

    this->ref_count = new int(1);
    this->parent_matrix = nullptr;
}

template <typename T>
inline Matrix<T>::Matrix(int nrows, int ncols, T fill) {
    this->nrows = nrows;
    this->ncols = ncols;
    this->data = new T[nrows * ncols];

    this->ref_count = new int(1);
    this->parent_matrix = nullptr;

    for (int i = 0; i < nrows * ncols; i++) {
        this->data[i] = fill;
    }
}

template <typename T>
inline Matrix<T>::Matrix(int nrows, int ncols, T* data) {
    this->nrows = nrows;
    this->ncols = ncols;
    this->data = new T[nrows * ncols];

    this->ref_count = new int(1);
    this->parent_matrix = nullptr;

    memcpy(this->data, data, nrows * ncols * sizeof(T));
}

template <typename T>
inline Matrix<T>::Matrix(int nrows, int ncols) {
    this->nrows = nrows;
    this->ncols = ncols;
    this->data = new T[nrows * ncols];

    this->ref_count = new int(1);
    this->parent_matrix = nullptr;
}

template <typename T>
inline Matrix<T>::Matrix(const Matrix<T>& other) {
    this->nrows = other.nrows;
    this->ncols = other.ncols;
    this->parent_matrix = other.parent_matrix;

    this->data = other.data;
    this->ref_count = other.ref_count;
    *(this->ref_count) += 1;  // do not use ++
}

template <typename T>
inline Matrix<T>::~Matrix() {
    // keep the fields for submatrix to use
    // this->nrows = 0;
    // this->ncols = 0;

    *(this->ref_count) -= 1;
    if (*(this->ref_count) == 0 && this->data != nullptr) {
        if (this->parent_matrix == nullptr) {
            delete[] this->data;
        } else {  // it is a submatrix
            delete[] this->parent_matrix->data;
        }
        delete this->ref_count;
    }

    // this->data = nullptr;
    // this->ref_count = nullptr;
}

template <typename T>
inline Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
    this->nrows = other.nrows;
    this->ncols = other.ncols;
    this->parent_matrix = other.parent_matrix;

    *(this->ref_count) -= 1;
    if (*(this->ref_count) == 0 && this->data != nullptr) {
        delete this->ref_count;
        delete[] this->data;
    }

    this->data = other.data;
    this->ref_count = other.ref_count;
    *(this->ref_count) += 1;

    return *this;
}

template <typename T>
inline Matrix<T> Matrix<T>::copy() {
    Matrix<T> res(this->nrows, this->ncols);

    memcpy(res.data, this->data, this->nrows * this->ncols * sizeof(T));

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::create_row_vec(int ncols, T fill) {
    return Matrix(1, ncols, fill);
}

template <typename T>
Matrix<T> Matrix<T>::create_col_vec(int nrows, T fill) {
    return Matrix(nrows, 1, fill);
}

template <typename T>
Matrix<T> Matrix<T>::create_diagonal(int nrows, T fill) {
    if (nrows < 1) {
        cout << "Diagonal matrix error: rows is less than 1" << endl;
    }
    Matrix<T> res = Matrix(nrows, nrows);

    for (int i = 0; i < nrows; i++) {
        res[i][i] = fill;
    }

    return res;
}

// --------------------------------------------------------------------------------------------

template <typename T>
Matrix<T> Matrix<T>::read_matrix(const char* file_name) {
    ifstream in(file_name);
    if (!in.is_open()) {
        cout << "Error opening file." << endl;
        exit(EXIT_FAILURE);
    }

    Matrix<T> m;

    // get ncols and nrows
    T temp = 0;
    char tempc = 1;
    while (tempc != '\n') {
        in >> temp;
        tempc = in.get();
        m.ncols++;
    }
    m.nrows++;
    while ((tempc = in.get()) != EOF) {
        if (tempc == '\n') {
            m.nrows++;
        }
    }
    m.data = new T[m.nrows * m.ncols];

    // read data
    in.clear();
    in.seekg(0, std::ios::beg);
    for (int i = 0; i < m.ncols * m.nrows; i++) {
        in >> m.data[i];
    }

    return m;
}

template <typename T>
void Matrix<T>::print(const char* file_name) {
    ofstream out(file_name);
    if (typeid(T) == typeid(double)) {
        out.precision(15);
    }

    for (int i = 0; i < this->nrows; i++) {
        for (int j = 0; j < this->ncols; j++) {
            out << (*this)[i][j] << " ";
        }
        out << endl;
    }

    out.close();
}

template <typename T>
void Matrix<T>::print() {
    if (typeid(T) == typeid(double)) {
        cout.precision(15);
    }

    for (int i = 0; i < this->nrows; i++) {
        for (int j = 0; j < this->ncols; j++) {
            cout << (*this)[i][j] << " ";
        }
        cout << endl;
    }
}

// --------------------------------------------------------------------------------------------

template <typename T>
inline T* Matrix<T>::operator[](int i) {
    if (i > this->nrows - 1) {
        cout << "Index error: array index out of bound." << endl;
        exit(EXIT_FAILURE);
    }

    if (this->parent_matrix != nullptr) {
        return this->data + i * this->parent_matrix->ncols;
    }
    return this->data + i * this->ncols;
}

template <typename T>
inline T& Matrix<T>::operator()(int i, int j) {
    if (i > this->nrows - 1 || j > this->ncols - 1) {
        cout << "Index error: array index out of bound." << endl;
        exit(EXIT_FAILURE);
    }
    
    if (this->parent_matrix != nullptr) {
        return this->data[i * this->parent_matrix->ncols + j];
    }

    return this->data[i * this->ncols + j];
}

template <typename T>
inline bool Matrix<T>::operator==(Matrix<T>& other) {
    if (this == &other) {
        return true;
    }

    if (!this->shape_equals(other)) {
        return false;
    }

    if (this->data == other.data) {
        return true;
    }

    for (int i = 0; i < this->nrows * this->ncols; i++) {
        if (this->data[i] != other.data[i]) {
            return false;
        }
    }

    return true;
}

template <typename T>
inline bool Matrix<T>::operator!=(Matrix<T>& other) {
    return !(this->operator==(other));
}

template <typename T>
inline Matrix<T> Matrix<T>::operator+(Matrix<T>& other) {
    if (!this->shape_equals(other)) {
        cout << "Addition error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    Matrix<T> res(this->nrows, this->ncols);

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] = this->data[i] + other.data[i];
    }

    return res;
}

template <typename T>
inline Matrix<T> Matrix<T>::operator-(Matrix<T>& other) {
    if (!this->shape_equals(other)) {
        cout << "Subtraction error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    Matrix<T> res(this->nrows, this->ncols);

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] = this->data[i] - other.data[i];
    }

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(Matrix& other) {
    if (this->shape_equals(other) && this->nrows == this->ncols && (this->nrows & (this->nrows - 1)) == 0) {
        return Matrix<T>::strassen(*this, other);
    }
    return Matrix<T>::multiply_matrix(*this, other);
}

template <typename T>
Matrix<T> Matrix<T>::operator*(T coef) {
    Matrix<T> res = this->copy();

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] *= coef;
    }

    return res;
}

template <typename U>
Matrix<U> operator*(int coef, Matrix<U>& m) {
    Matrix<U> res = m.copy();

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] *= coef;
    }

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator^(int expo) {
    if (this->nrows != this->ncols) {
        cout << "Power error: matrix is not square." << endl;
        exit(EXIT_FAILURE);
    }

    if (expo < 1) {
        cout << "Power error: exponent is less than 1." << endl;
        exit(EXIT_FAILURE);
    }

    Matrix<T> res = Matrix<T>::create_diagonal(this->nrows, 1);
    Matrix<T> temp = this->copy();

    while (expo) {
        if (expo & 1) {
            res *= temp;
        }
        temp *= temp;
        expo >>= 1;
    }

    return res;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(Matrix<T>& other) {
    *this = (*this) * other;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(T coef) {
    *this = (*this) * coef;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(Matrix<T>& other) {
    *this = (*this) + other;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(Matrix<T>& other) {
    *this = (*this) - other;
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::multiply_elements(Matrix<T>& other) {
    if (!this->shape_equals(other)) {
        cout << "Multiplication error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    Matrix<T> res(this->nrows, this->ncols);

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] = this->data[i] * other.data[i];
    }

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::multiply_matrix(Matrix<T>& m1, Matrix<T>& m2) {
    if (m1.ncols != m2.nrows) {
        cout << "Multiplication error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    Matrix<T> res(m1.nrows, m2.ncols, 0);

    for (int i = 0; i < m1.nrows; i++)
        for (int k = 0; k < m1.ncols; k++)
            for (int j = 0; j < m2.ncols; j++) {
                res[i][j] += m1[i][k] * m2[k][j];
            }

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::strassen(Matrix<T>& A, Matrix<T>& B) {
    if (!A.shape_equals(B)) {
        cout << "Strassen multiplication error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    int N = A.nrows;

    if ((N & (N - 1)) != 0) {
        cout << "Strassen multiplication error: matrix dimension is not 2^n." << endl;
        exit(EXIT_FAILURE);
    }

    if (N <= STRASSEN_LOWER_BOUND) {
        return Matrix<T>::multiply_matrix(A, B);
    }

    Matrix<T> A11 = A.submatrix_cpy(0, N / 2, 0, N / 2);
    Matrix<T> A12 = A.submatrix_cpy(0, N / 2, N / 2, N);
    Matrix<T> A21 = A.submatrix_cpy(N / 2, N, 0, N / 2);
    Matrix<T> A22 = A.submatrix_cpy(N / 2, N, N / 2, N);

    Matrix<T> B11 = B.submatrix_cpy(0, N / 2, 0, N / 2);
    Matrix<T> B12 = B.submatrix_cpy(0, N / 2, N / 2, N);
    Matrix<T> B21 = B.submatrix_cpy(N / 2, N, 0, N / 2);
    Matrix<T> B22 = B.submatrix_cpy(N / 2, N, N / 2, N);

    Matrix<T> S1 = B12 - B22;
    Matrix<T> S2 = A11 + A12;
    Matrix<T> S3 = A21 + A22;
    Matrix<T> S4 = B21 - B11;
    Matrix<T> S5 = A11 + A22;
    Matrix<T> S6 = B11 + B22;
    Matrix<T> S7 = A12 - A22;
    Matrix<T> S8 = B21 + B22;
    Matrix<T> S9 = A11 - A21;
    Matrix<T> S10 = B11 + B12;

    Matrix<T> P1 = strassen(A11, S1);
    Matrix<T> P2 = strassen(S2, B22);
    Matrix<T> P3 = strassen(S3, B11);
    Matrix<T> P4 = strassen(A22, S4);
    Matrix<T> P5 = strassen(S5, S6);
    Matrix<T> P6 = strassen(S7, S8);
    Matrix<T> P7 = strassen(S9, S10);

    Matrix<T> C11 = P5 + P4 - P2 + P6;
    Matrix<T> C12 = P1 + P2;
    Matrix<T> C21 = P3 + P4;
    Matrix<T> C22 = P5 + P1 - P3 - P7;

    Matrix<T> C = Matrix<T>::merge_matrix(C11, C12, C21, C22);

    return C;
}

// --------------------------------------------------------------------------------------------

template <typename T>
Matrix<T> Matrix<T>::submatrix_ROI(int row_start, int row_end, int col_start, int col_end) {
    Matrix<T> res;
    res.nrows = row_end - row_start;
    res.ncols = col_end - col_start;

    if (this->parent_matrix == nullptr) {
        res.data = this->data + row_start * this->ncols + col_start;
        res.parent_matrix = this;
    } else {
        res.data = this->data + row_start * this->parent_matrix->ncols + col_start;
        res.parent_matrix = this->parent_matrix;
    }

    res.ref_count = this->ref_count;
    *(res.ref_count) += 1;

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::submatrix_cpy(int row_start, int row_end, int col_start, int col_end) {
    Matrix<T> res(row_end - row_start, col_end - col_start);

    for (int i = row_start; i < row_end; i++)
        for (int j = col_start; j < col_end; j++) {
            res[i - row_start][j - col_start] = (*this)[i][j];
        }

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::submatrix(int row_start, int row_end, int col_start, int col_end) {
    if (row_start < 0 || row_end > this->nrows || col_start < 0 || col_end > this->ncols) {
        cout << "Submatrix error: array index out of bound." << endl;
        exit(EXIT_FAILURE);
    }

    return submatrix_ROI(row_start, row_end, col_start, col_end);
}

template <typename T>
void Matrix<T>::adjust_ROI(int row_start, int row_end, int col_start, int col_end) {
    if (this->parent_matrix == nullptr) {
        cout << "ROI adjustment error: not a submatrix." << endl;
        exit(EXIT_FAILURE);
    }

    if (row_start < 0 || row_end > this->parent_matrix->nrows || col_start < 0 || col_end > this->parent_matrix->ncols) {
        cout << "ROI adjustment error: array index out of bound." << endl;
        exit(EXIT_FAILURE);
    }

    this->nrows = row_end - row_start;
    this->ncols = col_end - col_start;
    this->data = this->parent_matrix->data + row_start * this->parent_matrix->ncols + col_start;
}

template <typename T>
Matrix<T> Matrix<T>::merge_matrix(Matrix<T>& C11, Matrix<T>& C12, Matrix<T>& C21, Matrix<T>& C22) {
    Matrix C(C11.nrows + C21.nrows, C11.ncols + C12.ncols);

    for (int i = 0; i < C11.nrows; i++)
        for (int j = 0; j < C11.ncols; j++) {
            C[i][j] = C11[i][j];
        }
    for (int i = 0; i < C12.nrows; i++)
        for (int j = 0; j < C12.ncols; j++) {
            C[i][j + C11.ncols] = C12[i][j];
        }
    for (int i = 0; i < C21.nrows; i++)
        for (int j = 0; j < C21.ncols; j++) {
            C[i + C11.nrows][j] = C21[i][j];
        }
    for (int i = 0; i < C22.nrows; i++)
        for (int j = 0; j < C22.ncols; j++) {
            C[i + C11.nrows][j + C11.ncols] = C22[i][j];
        }

    return C;
}
