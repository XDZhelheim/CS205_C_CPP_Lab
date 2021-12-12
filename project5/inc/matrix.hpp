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
    template <typename U> friend Matrix<U> operator*(int coef, Matrix<U>& m);

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

#endif
