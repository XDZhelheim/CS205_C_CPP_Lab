/**
 * @file matrix.hpp
 * @author Zheng Dong (11812804@mail.sustech.edu.cn) github.com/XDZhelheim
 * @brief `Matrix` class
 * @version 1.0.1
 * @date 2021-12-11
 * 
 * @copyright Copyright (c) 2021 Zheng Dong
 * 
 */

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <fstream>
#include <iostream>

/**
 * @brief Lower bound of the recursion in Strassen algorithm.
 * 
 * If the matrix size is less than it, switch to for-loop multiplication.
 * @see strassen(matrix<T>& A, matrix<T>& B)
 */
#define STRASSEN_LOWER_BOUND 128

/**
 * @brief `Matrix` class
 * 
 * A data structure for matrix. Contains some basic opeartions.
 * 
 * @tparam T Template type name.
 */
template <typename T>
class Matrix {
   private:
    // fields ---------------------------------------------------------------------------------------------------------------------------
    /**
     * @brief Number of rows
     */
    int nrows;

    /**
     * @brief Number of columns
     */
    int ncols;

    /**
     * @brief 1D array that stores data
     * 
     * Will be shared when perform copy construction or assignment operation (shallow copy). @see matrix(const matrix<T>& other) operator=(const matrix<T>& other)
     * 
     * Will be freed only when `ref_count` decreases to zero. @see ~matrix()
     */
    T* data;

    /**
     * @brief Pointer of a submatrix's parent
     * 
     * Should be `nullptr` except it is a submatrix.
     * @see submatrix_ROI(int row_start, int row_end, int col_start, int col_end)
     */
    const Matrix<T>* parent_matrix;

    /**
     * @brief Count the number of matrices that share the same `data`.
     * 
     * `data` will be freed only when `ref_count` decreases to zero.
     */
    int* ref_count;

   public:
    /**
     * @brief Get the number of rows.
     * @return int Number of rows.
     */
    int get_nrows();

    /**
     * @brief Get the number of columns.
     * @return int Number of columns.
     */
    int get_ncols();

    /**
     * @brief Get the pointer of array.
     * 
     * @return T* Pointer of array.
     */
    T* get_data();

    /**
     * @brief Get an array: `[nrows, ncols]`.
     * 
     * @return int* 
     */
    int* shape();

    /**
     * @brief Print `(nrows, ncols)` at `stdout`.
     */
    void print_shape();

    /**
     * @brief Judge if two matrices have the exactly same shape.
     * 
     * @param other Matrix that will be compared to.
     * @return true Same shape.
     * @return false Different shape.
     */
    bool shape_equals(Matrix<T>& other);

    // construction & destruction -------------------------------------------------------------------------------------------------------
    /**
     * @brief Construct a new matrix object.
     * 
     * `nrows` and `ncols` will be initialized to zero.
     * 
     * `data` will be initialized to `nullptr`.
     * 
     * `ref_count` will be allocated and the `int` value it points to will be set as 1.
     */
    Matrix();

    /**
     * @brief Construct a new matrix object by `nrows` and `ncols`.
     * 
     * `data` will be allocated. The length is `nrows * ncols`. But the values are not assigned, be very careful with that!
     * 
     * @warning Use it only when needing to manually set different values. And DO NOT perform any operation before value assignment.
     * 
     * `ref_count` will be allocated and the `int` value it points to will be set as 1.
     * 
     * @param nrows Number of rows.
     * @param ncols Number of columns.
     */
    Matrix(int nrows, int ncols);

    /**
     * @brief Construct a new matrix object.
     * 
     * `data` will be allocated. The length is `nrows * ncols`. And it will be filled with `fill`.
     * 
     * matrix(nrows, ncols, 0) is suggested when creating an empty matrix.
     * 
     * `ref_count` will be allocated and the `int` value it points to will be set as 1.
     * 
     * @param nrows Number of rows.
     * @param ncols Number of columns.
     * @param fill  Data that will be filled in the matrix.
     */
    Matrix(int nrows, int ncols, T fill);

    /**
     * @brief Construct a new Matrix object by given `data`.
     * 
     * Will use `memcpy` to copy elements in `data`.
     * 
     * @note Memory copy is must because of the management of `ref_count`.
     * 
     * @param nrows Number of rows.
     * @param ncols Number of columns.
     * @param data Given `data`.
     */
    Matrix(int nrows, int ncols, T* data);

    /**
     * @brief Copy constructor.
     * 
     * Shallow copy.
     * 
     * `data` will point to the same array as `other.data`. The two matrices will change together if any element is modified.
     * 
     * `ref_count` will also point to the same value as `other`. Then it increases by 1, which means there is another matrix that shares the same array.
     * 
     * @param other Matrix that copies from.
     */
    Matrix(const Matrix<T>& other);

    /**
     * @brief Destroy the matrix object when no refs left.
     * 
     * Decreases `ref_count` by 1.
     * 
     * If `ref_count` becomes zero, which means there is no matrix that use this array, then free `data` and `ref_count`.
     * 
     */
    ~Matrix();

    /**
     * @brief Make a hard copy of the caller.
     * 
     * `data` will be allocated as a new array. Its values will be assigned by for-loop.
     * 
     * `ref_count` will be allocated and the `int` value it points to will be set as 1.
     * 
     * @return matrix<T> The copy.
     */
    Matrix<T> copy();

    /**
     * @brief Assignment operator.
     * 
     * Shallow copy. Similar to copy constructor.
     * 
     * Will delete original `data` if `ref_count` is 1.
     * 
     * @param other Matrix that assigns from.
     * @return matrix<T>& LHS matrix. To enable continuous assignment such as A=B=C.
     */
    Matrix<T>& operator=(const Matrix<T>& other);

    /**
     * @brief Create a row vector.
     * 
     * Returns a row vector whose shape is `[1, ncols]`.
     * 
     * @param ncols Number of columns.
     * @param fill Data that will be filled in the vector.
     * @return matrix<T> Row vector.
     */
    static Matrix<T> create_row_vec(int ncols, T fill);

    /**
     * @brief Create a column vector.
     * 
     * Returns a column vector whose shape is `[nrows, 1]`.
     * 
     * @param nrows Number of rows.
     * @param fill Data that will be filled in the vector.
     * @return matrix<T> Column vector.
     */
    static Matrix<T> create_col_vec(int nrows, T fill);

    /**
     * @brief Create a diagonal matrix.
     * 
     * Returns a diagonal square matrix. The diagonal will be filled with `fill`.
     * 
     * @param nrows Number of rows and columns (square).
     * @param fill Data that will be filled in the diagonal.
     * @return matrix<T> Diagonal matrix.
     */
    static Matrix<T> create_diagonal(int nrows, T fill);

    // I/O ------------------------------------------------------------------------------------------------------------------------------
    /**
     * @brief Create a matrix from file.
     * 
     * @warning The file must have a trailing blank line.
     * 
     * @param file_name The path of the file.
     * @return matrix<T> Matrix constructed by the file.
     */
    static Matrix<T> read_matrix(const char* file_name);

    /**
     * @brief Print the matrix to file.
     * 
     * The format is:
     * ```
     * 1 2 3 4
     * 5 6 7 8
     * 
     * ```
     * 
     * @param file_name The path of the file.
     */
    void print(const char* file_name);

    /**
     * @brief Print the matrix at `stdout`.
     */
    void print();

    // operations -----------------------------------------------------------------------------------------------------------------------
    /**
     * @brief Index operator.
     * 
     * Since the values are stored in a 1D array, an operator is needed to get elements like `A[2][5]`.
     * 
     * This fuction returns the pointer at the start of the third row (example above).
     * Then by `[5]`, the 6-th element of the third row will be returned.
     * 
     * @param i Index.
     * @return T* The pointer to the start of the `(i+1)`-th row.
     */
    T* operator[](int i);

    /**
     * @brief Another way to get an element by two indexes.
     * 
     * `A(2, 5)` is equal to `A[2][5]`. Both are acceptable.
     * 
     * @note It is recommended to use () operator since it has a more robust boundary check.
     * 
     * @param i Row index.
     * @param j Column index.
     * @return T& The element.
     */
    T& operator()(int i, int j);

    /**
     * @brief Judge if two matrices are equal.
     * 
     * Two matrices is equal only when:
     * 1. Same data type
     * 2. Same shape
     * 3. Exactly same values, or `data` points to same address
     * 
     * @param other Matrix that compares to.
     * @return true Equal.
     * @return false Not equal.
     */
    bool operator==(Matrix<T>& other);

    /**
     * @brief Judge if two matrices are not equal.
     * 
     * A NOT version of operator==(matrix<T>& other).
     * 
     * @param other Matrix that compares to.
     * @return true Not equal.
     * @return false Equal.
     */
    bool operator!=(Matrix<T>& other);

    /**
     * @brief Matrix addition.
     * 
     * Add two matrices. Returns the result matrix.
     * 
     * @param other Adder.
     * @return matrix<T> Sum.
     */
    Matrix<T> operator+(Matrix<T>& other);

    /**
     * @brief Add a number and a matrix;
     * 
     * @param t Adder.
     * @return Matrix<T> Sum.
     */
    Matrix<T> operator+(T t);

    /**
     * @brief Matrix subtraction.
     * 
     * Subtract two matrices. Returns the result matrix.
     * 
     * @param other Subtracter.
     * @return matrix<T> Difference.
     */
    Matrix<T> operator-(Matrix<T>& other);

    /**
     * @brief Matrix multiplication.
     * 
     * Multiply two matrices. Returns the result matrix.
     * 
     * @warning The number of columns of multiplicand must be equal to the number of rows of multiplier.
     * 
     * Strassen algorithm will be used if they are square matrices and the shape is a power of two. @see strassen(matrix<T>& A, matrix<T>& B)
     * 
     * Otherwise, use for-loop multiplication. @see multiply_matrix(matrix<T>& m1, matrix<T>& m2)
     * 
     * @param other Multiplier.
     * @return matrix<T> Product.
     */
    Matrix<T> operator*(Matrix<T>& other);

    /**
     * @brief Multiply whole matrix by a number.
     * 
     * @note The data type of the coefficient will be coverted as the data type of the matrix.
     * Therefore, it is not recommended to multiply an `int` matrix by a floating point number.
     * 
     * @param coef Coefficient.
     * @return matrix<T> Product.
     */
    Matrix<T> operator*(T coef);

    /**
     * @brief Multiply an interger by a matrix.
     * 
     * @warning Only support `int` to avoid type deducing issues.
     * 
     * @tparam U Type name of the template friend function.
     * @param coef Coefficient.
     * @param m Matrix.
     * @return matrix<U> Product.
     */
    template <typename U>
    friend Matrix<U> operator*(int coef, Matrix<U>& m);

    /**
     * @brief Matrix power.
     * 
     * @warning Its shape must be square.
     * 
     * Will use fast power algorithm.
     * 
     * @param expo Exponent.
     * @return matrix<T> Power.
     */
    Matrix<T> operator^(int expo);

    /**
     * @brief Multiplication assignment by another matrix.
     * 
     * `m1 *= m2` is equal to `m1 = m1 * m2`.
     * 
     * @param other Multiplier.
     * @return matrix<T>& Product.
     */
    Matrix<T>& operator*=(Matrix<T>& other);

    /**
     * @brief Multiplication assignment by a number.
     * 
     * `m1 *= coef` is equal to `m1 = m1 * coef`.
     * 
     * @param coef Coefficient.
     * @return matrix<T>& Product.
     */
    Matrix<T>& operator*=(T coef);

    /**
     * @brief Addition assignment.
     * 
     * `m1 += m2` is equal to `m1 = m1 + m2`.
     * 
     * @param other Adder.
     * @return matrix<T>& Sum.
     */
    Matrix<T>& operator+=(Matrix<T>& other);

    /**
     * @brief Add a number to matrix;
     * 
     * @param t Adder.
     * @return Matrix<T> Sum.
     */
    Matrix<T>& operator+=(T t);

    /**
     * @brief Subtraction assignment.
     * 
     * `m1 -= m2` is equal to `m1 = m1 - m2`.
     * 
     * @param other Subtracter.
     * @return matrix<T>& Difference.
     */
    Matrix<T>& operator-=(Matrix<T>& other);

    /**
     * @brief Multiply the elements of two matrices.
     * 
     * This function multiplies the value at the same position. eg.`A[i][j] * B[i][j]`.
     * 
     * Requires same shape.
     * 
     * @param other Multiplier.
     * @return matrix<T> Product.
     */
    Matrix<T> multiply_elements(Matrix<T>& other);

    /**
     * @brief Get the sum of the matrix elements.
     * 
     * @return T Sum of elements.
     */
    T sum();

    /**
     * @brief Get the maximum of the matrix elements.
     * 
     * @return T Maximum of elements;
     */
    T max();

    /**
     * @brief Pad a matrix with 0.
     * 
     * @param length Pad length.
     * @return Matrix<T> Matrix after padding.
     */
    Matrix<T> pad(int length);

    /**
     * @brief Convolution.
     * 
     * @param kernel Convolution kernel.
     * @param pad 0: valid mode; 1: same mode
     * @param stride Stride. Out size = in size / stride (same mode).
     * @return Matrix<T> Convolution result.
     */
    Matrix<T> convolution(Matrix<T>& kernel, bool padding, int stride);

    /**
     * @brief For-loop matrix multiplication.
     * 
     * Time complexity: $O(n^3)$
     * 
     * @param m1 Multiplicand.
     * @param m2 Multiplier.
     * @return matrix<T> Product.
     */
    static Matrix<T> multiply_matrix(Matrix<T>& m1, Matrix<T>& m2);

    /**
     * @brief Strassen algorithm for matrix multiplication.
     * 
     * Time complexity: approx. $O(n^{2.7})$
     * 
     * When the size of the matrix is less or equal than STRASSEN_LOWER_BOUND, it will switch to for-loop multiplication.
     * 
     * @param A Multiplicand.
     * @param B Multiplier.
     * @return matrix<T> Product.
     */
    static Matrix<T> strassen(Matrix<T>& A, Matrix<T>& B);

    // submatrix and merge matrix -------------------------------------------------------------------------------------------------------
    /**
     * @brief Get a submatrix with ROI concept.
     * 
     * Create a submatrix by set the `data` pointer to the specified position.
     * 
     * Therefore, the submatrix shares the same array with parent.
     * 
     * As a result, if submatrix is modified, the value in parent matrix will be changed together.
     * 
     * `parent_matrix` pointer will be set.
     * 
     * @note [row_start, row_end) [col_start, col_end)
     * 
     * @param row_start Top left corner.
     * @param row_end Top right corner.
     * @param col_start Bottom left corner.
     * @param col_end Bottom right corner.
     * @return matrix<T> Submatrix.
     */
    Matrix<T> submatrix_ROI(int row_start, int row_end, int col_start, int col_end);

    /**
     * @brief Create a submatrix by hard copy.
     * 
     * Create a new matrix, and assign values from parent.
     * 
     * @note `parent_matrix` will not be set.
     * 
     * @note [row_start, row_end) [col_start, col_end)
     * 
     * @param row_start Top left corner.
     * @param row_end Top right corner.
     * @param col_start Bottom left corner.
     * @param col_end Bottom right corner.
     * @return matrix<T> Submatrix.
     */
    Matrix<T> submatrix_cpy(int row_start, int row_end, int col_start, int col_end);

    /**
     * @brief Alias of submatrix_ROI.
     * 
     * @note [row_start, row_end) [col_start, col_end)
     * 
     * @param row_start Top left corner.
     * @param row_end Top right corner.
     * @param col_start Bottom left corner.
     * @param col_end Bottom right corner.
     * @return matrix<T> Submatrix.
     */
    Matrix<T> submatrix(int row_start, int row_end, int col_start, int col_end);

    /**
     * @brief Adjust the location of ROI.
     * 
     * Must be a submatrix, i.e. `parent_matrix` is not `nullptr`.
     * 
     * @param row_start New top left corner.
     * @param row_end New top right corner.
     * @param col_start New bottom left corner.
     * @param col_end New bottom right corner.
     */
    void adjust_ROI(int row_start, int row_end, int col_start, int col_end);

    /**
     * @brief Merge four submatrices.
     * 
     * Mainly used in Strassen algorithm.
     * @see strassen(matrix<T>& A, matrix<T>& B)
     * 
     * @note [row_start, row_end) [col_start, col_end)
     * 
     * @param C11 Top left submatrix.
     * @param C12 Top right submatrix.
     * @param C21 Bottom left submatrix.
     * @param C22 Bottom right submatrix.
     * @return matrix<T> Merged matrix.
     */
    static Matrix<T> merge_matrix(Matrix<T>& C11, Matrix<T>& C12, Matrix<T>& C21, Matrix<T>& C22);
};

// ----------------------------------------------------------------------------------------------------------------------

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
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Diagonal matrix error: rows is less than 1" << endl;
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
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Index error: array index out of bound." << endl;
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
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Index error: array index out of bound." << endl;
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

    for (int i = 0; i < this->nrows; i++) {
        for (int j = 0; j < this->ncols; j++) {
            if (this->operator()(i, j) != other[i][j]) {
                return false;
            }
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
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Addition error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    Matrix<T> res(this->nrows, this->ncols);

    for (int i = 0; i < res.nrows; i++) {
        for (int j = 0; j < res.ncols; j++) {
            res[i][j] = (*this)[i][j] + other[i][j];
        }
    }

    return res;
}

template <typename T>
inline Matrix<T> Matrix<T>::operator+(T t) {
    Matrix<T> res(this->nrows, this->ncols);

    for (int i = 0; i < res.nrows; i++) {
        for (int j = 0; j < res.ncols; j++) {
            res[i][j] = (*this)[i][j] + t;
        }
    }

    return res;
}

template <typename T>
inline Matrix<T> Matrix<T>::operator-(Matrix<T>& other) {
    if (!this->shape_equals(other)) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Subtraction error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    Matrix<T> res(this->nrows, this->ncols);

    for (int i = 0; i < res.nrows; i++) {
        for (int j = 0; j < res.ncols; j++) {
            res[i][j] = (*this)[i][j] - other[i][j];
        }
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

    for (int i = 0; i < res.nrows; i++) {
        for (int j = 0; j < res.ncols; j++) {
            res[i][j] *= coef;
        }
    }

    return res;
}

template <typename U>
Matrix<U> operator*(int coef, Matrix<U>& m) {
    Matrix<U> res = m.copy();

    for (int i = 0; i < res.nrows; i++) {
        for (int j = 0; j < res.ncols; j++) {
            res[i][j] *= coef;
        }
    }

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator^(int expo) {
    if (this->nrows != this->ncols) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Power error: matrix is not square." << endl;
        exit(EXIT_FAILURE);
    }

    if (expo < 1) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Power error: exponent is less than 1." << endl;
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
    for (int i = 0; i < this->nrows; i++) {
        for (int j = 0; j < this->ncols; j++) {
            (*this)[i][j] *= coef;
        }
    }

    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(Matrix<T>& other) {
    if (!this->shape_equals(other)) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Addition error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < this->nrows; i++) {
        for (int j = 0; j < this->ncols; j++) {
            (*this)[i][j] += other[i][j];
        }
    }

    return *this;
}

template <typename T>
inline Matrix<T>& Matrix<T>::operator+=(T t) {
    for (int i = 0; i < this->nrows; i++) {
        for (int j = 0; j < this->ncols; j++) {
            (*this)[i][j] += t;
        }
    }

    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(Matrix<T>& other) {
    if (!this->shape_equals(other)) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Addition error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < this->nrows; i++) {
        for (int j = 0; j < this->ncols; j++) {
            (*this)[i][j] -= other[i][j];
        }
    }

    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::multiply_elements(Matrix<T>& other) {
    if (!this->shape_equals(other)) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Multiplication error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    Matrix<T> res(this->nrows, this->ncols);

    for (int i = 0; i < res.nrows; i++) {
        for (int j = 0; j < res.ncols; j++) {
            res[i][j] = (*this)[i][j] * other[i][j];
        }
    }

    return res;
}

template <typename T>
T Matrix<T>::sum() {
    T res = 0;

    for (int i = 0; i < this->nrows; i++) {
        for (int j = 0; j < this->ncols; j++) {
            res += (*this)[i][j];
        }
    }

    return res;
}

template <typename T>
T Matrix<T>::max() {
    T res = this->data[0];

    for (int i = 0; i < this->nrows; i++) {
        for (int j = 0; j < this->ncols; j++) {
            if ((*this)[i][j] > res) {
                res = (*this)[i][j];
            }
        }
    }

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::pad(int length) {
    Matrix<T> res(this->nrows + length * 2, this->ncols + length * 2);

    for (int i = 0; i < res.nrows; i++) {
        for (int j = 0; j < res.ncols; j++) {
            if (i < length || j < length || i >= this->nrows + length || j >= this->ncols + length) {
                res[i][j] = 0;
            } else {
                res[i][j] = (*this)[i - length][j - length];
            }
        }
    }

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::convolution(Matrix<T>& kernel, bool padding, int stride) {
    if (this->nrows != this->ncols) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Convolution error: this is not a square matrix." << endl;
        exit(EXIT_FAILURE);
    }

    if (kernel.nrows != kernel.ncols) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Convolution error: kernel is not a square matrix." << endl;
        exit(EXIT_FAILURE);
    }

    int size = this->nrows;
    int kernel_size = kernel.nrows;

    Matrix<T> res((size + 2 * padding - kernel_size) / stride + 1, (size + 2 * padding - kernel_size) / stride + 1, (T)0);
    cout << "conv res size = " << res.nrows << endl;

    if (padding) {
        // pad_length = (kernel_size - 1) / 2
        if (kernel_size % 2 == 0) {
            cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
                 << "Convolution error: padding requires odd size kernel." << endl;
            exit(EXIT_FAILURE);
        }

        Matrix<T> pad_matrix = this->pad((kernel_size - 1) / 2);

        int pad_matrix_size = pad_matrix.nrows;

        int res_i = 0;
        for (int i = 0; i <= pad_matrix_size - kernel_size; i += stride) {
            int res_j = 0;
            for (int j = 0; j <= pad_matrix_size - kernel_size; j += stride) {
                res[res_i][res_j] = pad_matrix.submatrix_ROI(i, i + kernel_size, j, j + kernel_size).multiply_elements(kernel).sum();
                res_j++;
            }
            res_i++;
        }
    } else {
        int res_i = 0;
        for (int i = 0; i <= size - kernel_size; i += stride) {
            int res_j = 0;
            for (int j = 0; j <= size - kernel_size; j += stride) {
                res[res_i][res_j] = this->submatrix_ROI(i, i + kernel_size, j, j + kernel_size).multiply_elements(kernel).sum();
                res_j++;
            }
            res_i++;
        }
    }

    return res;
}

template <typename T>
Matrix<T> Matrix<T>::multiply_matrix(Matrix<T>& m1, Matrix<T>& m2) {
    if (m1.ncols != m2.nrows) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Multiplication error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    Matrix<T> res(m1.nrows, m2.ncols, (T)0);

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
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Strassen multiplication error: matrix dimension cannot match." << endl;
        exit(EXIT_FAILURE);
    }

    int N = A.nrows;

    if ((N & (N - 1)) != 0) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Strassen multiplication error: matrix dimension is not 2^n." << endl;
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
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "Submatrix error: array index out of bound." << endl;
        exit(EXIT_FAILURE);
    }

    return submatrix_ROI(row_start, row_end, col_start, col_end);
}

template <typename T>
void Matrix<T>::adjust_ROI(int row_start, int row_end, int col_start, int col_end) {
    if (this->parent_matrix == nullptr) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "ROI adjustment error: not a submatrix." << endl;
        exit(EXIT_FAILURE);
    }

    if (row_start < 0 || row_end > this->parent_matrix->nrows || col_start < 0 || col_end > this->parent_matrix->ncols) {
        cout << __FILE__ << ": " << __FUNCTION__ << " at line " << __LINE__ << ": "
             << "ROI adjustment error: array index out of bound." << endl;
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

#endif
