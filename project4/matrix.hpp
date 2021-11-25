/**
 * @file matrix.hpp
 * @author Zheng Dong (11812804@mail.sustech.edu.cn) github.com/XDZhelheim
 * @brief Declarations of `matrix` class
 * @version 1.0.0
 * @date 2021-11-24
 * 
 * @copyright Copyright (c) 2021 Zheng Dong
 * 
 */

#pragma once

#include <fstream>
#include <iostream>

using namespace std;

/**
 * @brief Lower bound of the recursion in Strassen algorithm.
 * 
 * If the matrix size is less than it, switch to for-loop multiplication.
 * @see strassen(matrix<T>& A, matrix<T>& B)
 */
#define STRASSEN_LOWER_BOUND 128

/**
 * @brief `matrix` class
 * @tparam T Template type name.
 */
template <typename T>
class matrix {
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
     * Will be freed only when `ref_count` decrases to zero. @see ~matrix()
     */
    T* data;

    /**
     * @brief Pointer of a submatrix's parent
     * 
     * Should be `nullptr` except it is a submatrix.
     * @see submatrix_ROI(int row_start, int row_end, int col_start, int col_end)
     */
    const matrix<T>* parent_matrix;

    /**
     * @brief Count the number of matrices that share the same `data`.
     * 
     * `data` will be freed only when `ref_count` decrases to zero.
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
    bool shape_equals(matrix<T>& other);

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
    matrix();

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
    matrix(int nrows, int ncols);

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
    matrix(int nrows, int ncols, T fill);

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
    matrix(const matrix<T>& other);

    /**
     * @brief Destroy the matrix object when no refs left.
     * 
     * Decreases `ref_count` by 1.
     * 
     * If `ref_count` becomes zero, which means there is no matrix that use this array, then free `data` and `ref_count`.
     * 
     */
    ~matrix();

    /**
     * @brief Make a hard copy of the caller.
     * 
     * `data` will be allocated as a new array. Its values will be assigned by for-loop.
     * 
     * `ref_count` will be allocated and the `int` value it points to will be set as 1.
     * 
     * @return matrix<T> The copy.
     */
    matrix<T> copy();

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
    matrix<T>& operator=(const matrix<T>& other);

    /**
     * @brief Create a row vector.
     * 
     * Returns a row vector whose shape is `[1, ncols]`.
     * 
     * @param ncols Number of columns.
     * @param fill Data that will be filled in the vector.
     * @return matrix<T> Row vector.
     */
    static matrix<T> create_row_vec(int ncols, T fill);

    /**
     * @brief Create a column vector.
     * 
     * Returns a column vector whose shape is `[nrows, 1]`.
     * 
     * @param nrows Number of rows.
     * @param fill Data that will be filled in the vector.
     * @return matrix<T> Column vector.
     */
    static matrix<T> create_col_vec(int nrows, T fill);

    /**
     * @brief Create a diagonal matrix.
     * 
     * Returns a diagonal square matrix. The diagonal will be filled with `fill`.
     * 
     * @param nrows Number of rows and columns (square).
     * @param fill Data that will be filled in the diagonal.
     * @return matrix<T> Diagonal matrix.
     */
    static matrix<T> create_diagonal(int nrows, T fill);

    // I/O ------------------------------------------------------------------------------------------------------------------------------
    /**
     * @brief Create a matrix from file.
     * 
     * @warning The file must have a trailing blank line.
     * 
     * @param file_name The path of the file.
     * @return matrix<T> Matrix constructed by the file.
     */
    static matrix<T> read_matrix(const char* file_name);

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
     * @param i Row index.
     * @param j Column index.
     * @return T& The element.
     */
    T& operator()(int i, int j);

    /**
     * @brief Matrix addition.
     * 
     * Add two matrices. Returns the result matrix.
     * 
     * @param other Adder.
     * @return matrix<T> Sum.
     */
    matrix<T> operator+(matrix<T>& other);

    /**
     * @brief Matrix subtraction.
     * 
     * Subtract two matrices. Returns the result matrix.
     * 
     * @param other Subtracter.
     * @return matrix<T> Difference.
     */
    matrix<T> operator-(matrix<T>& other);

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
    matrix<T> operator*(matrix<T>& other);

    /**
     * @brief Multiply whole matrix by a number.
     * 
     * @note The data type of the coefficient will be coverted as the data type of the matrix.
     * Therefore, it is not recommended to multiply an `int` matrix by a floating point number.
     * 
     * @param coef Coefficient.
     * @return matrix<T> Product.
     */
    matrix<T> operator*(T coef);

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
    template <typename U> friend matrix<U> operator*(int coef, matrix<U>& m);

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
    matrix<T> operator^(int expo);

    /**
     * @brief Multiplication assignment by another matrix.
     * 
     * `m1 *= m2` is equal to `m1 = m1 * m2`.
     * 
     * @param other Multiplier.
     * @return matrix<T>& Product.
     */
    matrix<T>& operator*=(matrix<T>& other);

    /**
     * @brief Multiplication assignment by a number.
     * 
     * `m1 *= coef` is equal to `m1 = m1 * coef`.
     * 
     * @param coef Coefficient.
     * @return matrix<T>& Product.
     */
    matrix<T>& operator*=(T coef);

    /**
     * @brief Addition assignment.
     * 
     * `m1 += m2` is equal to `m1 = m1 + m2`.
     * 
     * @param other Adder.
     * @return matrix<T>& Sum.
     */
    matrix<T>& operator+=(matrix<T>& other);

    /**
     * @brief Subtraction assignment.
     * 
     * `m1 -= m2` is equal to `m1 = m1 - m2`.
     * 
     * @param other Subtracter.
     * @return matrix<T>& Difference.
     */
    matrix<T>& operator-=(matrix<T>& other);

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
    matrix<T> multiply_elements(matrix<T>& other);

    /**
     * @brief For-loop matrix multiplication.
     * 
     * Time complexity: $O(n^3)$
     * 
     * @param m1 Multiplicand.
     * @param m2 Multiplier.
     * @return matrix<T> Product.
     */
    static matrix<T> multiply_matrix(matrix<T>& m1, matrix<T>& m2);

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
    static matrix<T> strassen(matrix<T>& A, matrix<T>& B);

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
    matrix<T> submatrix_ROI(int row_start, int row_end, int col_start, int col_end);

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
    matrix<T> submatrix_cpy(int row_start, int row_end, int col_start, int col_end);

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
    matrix<T> submatrix(int row_start, int row_end, int col_start, int col_end);

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
    static matrix<T> merge_matrix(matrix<T>& C11, matrix<T>& C12, matrix<T>& C21, matrix<T>& C22);
};
