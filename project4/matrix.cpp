#include "matrix.hpp"

template <typename T>
inline int* matrix<T>::shape() {
    return new int[2]{this->nrows, this->ncols};
}

template <typename T>
inline void matrix<T>::print_shape() {
    cout << "(" << this->nrows << ", " << this->ncols << ")" << endl;
}

template <typename T>
inline bool matrix<T>::shape_equals(matrix<T>& other) {
    return this->nrows == other.nrows && this->ncols == other.ncols;
}

// ---------------------------------------------------------------------------------
template <typename T>
inline matrix<T>::matrix() {
    this->nrows = 0;
    this->ncols = 0;
    this->data = nullptr;
    this->parent_matrix = nullptr;
}

template <typename T>
inline matrix<T>::matrix(int nrows, int ncols, T fill) {
    this->nrows = nrows;
    this->ncols = ncols;
    this->data = new T[nrows * ncols];

    this->parent_matrix = nullptr;

    for (int i = 0; i < nrows * ncols; i++) {
        this->data[i] = fill;
    }
}

template <typename T>
inline matrix<T>::matrix(int nrows, int ncols) {
    this->nrows = nrows;
    this->ncols = ncols;
    this->data = new T[nrows * ncols];

    this->parent_matrix = nullptr;
}

template <typename T>
inline matrix<T>::matrix(const matrix<T>& other) {
    this->nrows = other.nrows;
    this->ncols = other.ncols;

    // set parent matrix to avoid freeing the array twice
    this->data = other.data;
    this->parent_matrix = &other;

    // this->data = new T[nrows * ncols];
    // for (int i = 0; i < this->nrows * this->ncols; i++) {
    //     this->data[i] = other.data[i];
    // }
}

template <typename T>
inline matrix<T>::~matrix() {
    if (this->parent_matrix == nullptr && this->data != nullptr) {
        this->nrows = 0;
        this->ncols = 0;
        delete[] this->data;
        this->data = nullptr;
    }
}

template <typename T>
inline matrix<T>& matrix<T>::operator=(matrix<T>& other) {
    this->nrows = other.nrows;
    this->ncols = other.ncols;

    if (this->parent_matrix == nullptr && this->data != nullptr) {
        delete[] this->data;
    }

    this->data = other.data;
    this->parent_matrix = &other;

    return *this;
}

template <typename T>
inline matrix<T> matrix<T>::copy() {
    matrix<T> res(this->nrows, this->ncols);

    for (int i = 0; i < this->nrows * this->ncols; i++) {
        res.data[i] = this->data[i];
    }

    return res;
}

template <typename T>
matrix<T> matrix<T>::create_row_vec(int ncols, T fill) {
    return matrix(0, ncols, fill);
}

template <typename T>
matrix<T> matrix<T>::create_col_vec(int nrows, T fill) {
    return matrix(nrows, 0, fill);
}

template <typename T>
matrix<T> matrix<T>::create_diagonal(int nrows, T fill) {
    matrix<T> m = matrix(nrows, nrows);

    for (int i = 0; i < nrows; i++) {
        m[i][i] = fill;
    }
}

// --------------------------------------------------------------------------------------------

template <typename T>
ostream& operator<<(ostream& out, const matrix<T>& m) {
    if (typeid(T) == typeid(double)) {
        out.precision(15);
    }

    for (int i = 0; i < m.nrows; i++) {
        for (int j = 0; j < m.ncols; j++) {
            out << m[i][j] << " ";
        }
        out << endl;
    }

    return out;
}

template <typename T>
matrix<T> matrix<T>::read_matrix(const char* file_name) {
    ifstream in(file_name);
    if (!in.is_open()) {
        cout << "Error opening file." << endl;
        exit(EXIT_FAILURE);
    }

    matrix<T> m;

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
    in.seekg(0, ios::beg);
    for (int i = 0; i < m.ncols * m.nrows; i++) {
        in >> m.data[i];
    }

    return m;
}

template <typename T>
void matrix<T>::print(const char* file_name) {
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

// --------------------------------------------------------------------------------------------

template <typename T>
inline T* matrix<T>::operator[](int i) {
    if (i > this->nrows) {
        printf("Index error: array index out of bound.\n");
        exit(EXIT_FAILURE);
    }

    if (this->parent_matrix != nullptr) {
        return this->data + i * this->parent_matrix->ncols;
    }
    return this->data + i * this->ncols;
}

template <typename T>
inline T& matrix<T>::operator()(int i, int j) {
    if (i > this->nrows || j > this->ncols) {
        printf("Index error: array index out of bound.\n");
        exit(EXIT_FAILURE);
    }
    if (this->parent_matrix != nullptr) {
        return this->data[i * this->parent_matrix->ncols + j];
    }

    return this->data[i * this->ncols + j];
}

template <typename T>
inline matrix<T> matrix<T>::operator+(matrix<T>& other) {
    if (!this->shape_equals(other)) {
        printf("Addition error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    matrix<T> res(this->nrows, this->ncols);

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] = this->data[i] + other.data[i];
    }

    return res;
}

template <typename T>
inline matrix<T> matrix<T>::operator-(matrix<T>& other) {
    if (!this->shape_equals(other)) {
        printf("Subtraction error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    matrix<T> res(this->nrows, this->ncols);

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] = this->data[i] - other.data[i];
    }

    return res;
}

template <typename T>
matrix<T> matrix<T>::operator*(matrix& other) {
    if (this->shape_equals(other) && this->nrows == this->ncols && (this->nrows & (this->nrows - 1)) == 0) {
        cout << "Using Strassen algorithm." << endl;
        return matrix<T>::strassen(*this, other);
    }
    return matrix<T>::multiply_matrix(*this, other);
}

template <typename T>
matrix<T> matrix<T>::operator^(int expo) {
    if (this->nrows != this->ncols) {
        printf("Power error: matrix is not square.\n");
        exit(EXIT_FAILURE);
    }

    if (expo < 1) {
        printf("Power error: exponent is less than 1.\n");
        exit(EXIT_FAILURE);
    }

    matrix<T> res = matrix<T>::create_diagonal(this->nrows, 1);
    matrix<T> temp = this->copy();

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
matrix<T>& matrix<T>::operator*=(matrix<T>& other) {
    *this = (*this) * other;
    return *this;
}

template <typename T>
matrix<T> matrix<T>::multiply_elements(matrix<T>& other) {
    if (!this->shape_equals(other)) {
        printf("Multiplication error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    matrix<T> res(this->nrows, this->ncols);

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] = this->data[i] * other.data[i];
    }

    return res;
}

template <typename T>
matrix<T> matrix<T>::multiply_matrix(matrix<T>& m1, matrix<T>& m2) {
    if (m1.ncols != m2.nrows) {
        printf("Multiplication error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    matrix<T> res(m1.nrows, m2.ncols, 0);

    for (int i = 0; i < m1.nrows; i++)
        for (int k = 0; k < m1.ncols; k++)
            for (int j = 0; j < m2.ncols; j++) {
                res[i][j] += m1[i][k] * m2[k][j];
            }

    return res;
}

template <typename T>
matrix<T> matrix<T>::strassen(matrix<T>& A, matrix<T>& B) {
    if (!A.shape_equals(B)) {
        printf("Strassen multiplication error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    int N = A.nrows;

    if ((N & (N - 1)) != 0) {
        printf("Strassen multiplication error: matrix dimension is not 2^n.\n");
        exit(EXIT_FAILURE);
    }

    if (N <= STRASSEN_LOWER_BOUND) {
        return matrix<T>::multiply_matrix(A, B);
    }

    matrix<T> A11 = A.submatrix(0, N / 2, 0, N / 2);
    matrix<T> A12 = A.submatrix(0, N / 2, N / 2, N);
    matrix<T> A21 = A.submatrix(N / 2, N, 0, N / 2);
    matrix<T> A22 = A.submatrix(N / 2, N, N / 2, N);

    matrix<T> B11 = B.submatrix(0, N / 2, 0, N / 2);
    matrix<T> B12 = B.submatrix(0, N / 2, N / 2, N);
    matrix<T> B21 = B.submatrix(N / 2, N, 0, N / 2);
    matrix<T> B22 = B.submatrix(N / 2, N, N / 2, N);

    matrix<T> S1 = B12 - B22;
    matrix<T> S2 = A11 + A12;
    matrix<T> S3 = A21 + A22;
    matrix<T> S4 = B21 - B11;
    matrix<T> S5 = A11 + A22;
    matrix<T> S6 = B11 + B22;
    matrix<T> S7 = A12 - A22;
    matrix<T> S8 = B21 + B22;
    matrix<T> S9 = A11 - A21;
    matrix<T> S10 = B11 + B12;

    matrix<T> P1 = strassen(A11, S1);
    matrix<T> P2 = strassen(S2, B22);
    matrix<T> P3 = strassen(S3, B11);
    matrix<T> P4 = strassen(A22, S4);
    matrix<T> P5 = strassen(S5, S6);
    matrix<T> P6 = strassen(S7, S8);
    matrix<T> P7 = strassen(S9, S10);

    matrix<T> C11 = P5 + P4 - P2 + P6;
    matrix<T> C12 = P1 + P2;
    matrix<T> C21 = P3 + P4;
    matrix<T> C22 = P5 + P1 - P3 - P7;

    matrix<T> C = matrix<T>::merge_matrix(C11, C12, C21, C22);

    return C;
}

// --------------------------------------------------------------------------------------------

template <typename T>
matrix<T> matrix<T>::submatrix_ROI(int row_start, int row_end, int col_start, int col_end) {
    matrix<T> res;
    res.nrows = row_end - row_start;
    res.ncols = col_end - col_start;

    res.data = this->data + row_start * this->ncols + col_start;
    res.parent_matrix = this;

    return res;
}

template <typename T>
matrix<T> matrix<T>::submatrix_cpy(int row_start, int row_end, int col_start, int col_end) {
    matrix<T> res(row_end - row_start, col_end - col_start);

    for (int i = row_start; i < row_end; i++)
        for (int j = col_start; j < col_end; j++) {
            res[i - row_start][j - col_start] = (*this)[i][j];
        }

    return res;
}

template <typename T>
matrix<T> matrix<T>::submatrix(int row_start, int row_end, int col_start, int col_end) {
    return submatrix_ROI(row_start, row_end, col_start, col_end);
}

template <typename T>
void matrix<T>::adjust_ROI(int row_start, int row_end, int col_start, int col_end) {
    if (this->parent_matrix == nullptr) {
        cout << "ROI adjustment error: not a submatrix." << endl;
        exit(EXIT_FAILURE);
    }

    this->data = this->parent_matrix->data + row_start * this->parent_matrix->ncols + col_start;
}

template <typename T>
matrix<T> matrix<T>::merge_matrix(matrix<T>& C11, matrix<T>& C12, matrix<T>& C21, matrix<T>& C22) {
    matrix C(C11.nrows + C21.nrows, C11.ncols + C12.ncols);

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
