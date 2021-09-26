# CS205 Project 2 Report

11812804 董正

---

### 1 Introduction

#### 1.1 Project Description

This project aims to implement a program to multiply two matrices in two files.

The requirements are:

1. Get file path from command line arguments

2. Implement the matrix multiplication in `float` and `double` separately

   Compare their speed and accuracy

3. Improve speed

In my code, I implemented **Strassen's algorithm** with `vector` in `C++`.

#### 1.2 Development Environment

* `Windows 10 Home China x86_64`
* Kernel version `10.0.19042`
* `g++.exe (tdm64-1) 10.3.0`
* C++ standard: `c++11`

---

### 2 Design and Implementation

Header files and macros used in this section:

```c++
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#define USE_DOUBLE 1

#if USE_DOUBLE
    #define REAL_NUMBER double
    #define PRECISION 15
#else
    #define REAL_NUMBER float
    #define PRECISION 6
#endif

#define STRASSEN_LOWER_BOUND 128

typedef vector<vector<REAL_NUMBER>> matrix;
```

#### 2.1 Matrix Construction

Since there is no size told in the files, it is necessary to compute the size of the input matrix.

For convenience, I used 2-dimension `vector` as the data structure for matrix.

To read a matrix, keep appending new numbers to the row vector. In the meantime, use `get()` function to test if it reaches `\n` and create a new vector. And if there is a blank line at the end of the file, which means `\nEOF`, do nothing. Use `peek()` to achieve this.

**Implementation:**

```c++
matrix read_matrix(const char *file_name) {
    ifstream in(file_name);
    if (!in.is_open()) {
        cout << "Error opening file." << endl;
        exit(EXIT_FAILURE);
    }

    matrix m;

    REAL_NUMBER temp;
    int i = 0, j = 0;
    m.push_back(vector<REAL_NUMBER>());
    while (in >> temp) {
        m[i].push_back(temp);
        if (in.get() == '\n' && in.peek() != EOF) {
            m.push_back(vector<REAL_NUMBER>());
            i++;
            j = 0;
        }
    }

    in.close();
    return m;
}
```

#### 2.2 Write Matrix to File

By default, the significant digits of a `float` or `double` number is 6. So it is essential to set the precison of the out stream. For example, 15 for `double`.

**Implementation:**

```c++
void print_matrix(matrix m, const char *file_name) {
    ofstream out(file_name);
    out.precision(PRECISION);

    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].size(); j++) {
            out << m[i][j] << " ";
        }
        out << endl;
    }

    out.close();
}
```

---

#### 2.3 For-loop Matrix Multiplication

The naive algorithm for matrix multiplication is to use a triple nested for-loop.
$$
C_{ij}=\sum_{k=1}^n A_{ik}\cdot B_{kj}
$$
**Implementation:**

```c++
matrix multiply_matrix(matrix m1, matrix m2) {
    // a*b dot b*c = a*c
    int nrows = m1.size();
    int ncols = m2[0].size();
    int intermediate = m2.size();

    if (intermediate != m1[0].size()) {
        cout << "Multiplication error: matrix dimension cannot match."
             << " (" << m1.size() << "*" << m1[0].size() << " dot " << m2.size()
             << "*" << m2[0].size() << ")" << endl;
        exit(EXIT_FAILURE);
    }

    matrix product(nrows, vector<REAL_NUMBER>(ncols, 0));

    for (int i = 0; i < nrows; i++)
        for (int k = 0; k < intermediate; k++)
            for (int j = 0; j < ncols; j++) {
                product[i][j] += m1[i][k] * m2[k][j];
            }

    return product;
}
```

Here I swapped the order of for-$k$ and for-$j$ in order to accelerate the speed. 

By swapping $k$ and $j$, the program can read memory consecutively, which can save memory access time.

---

#### 2.4 Strassen's Algorithm

Hence the dimension of the given matrices are a power of two, we can use **Strassen's algorithm** to increase the speed.

Volker Strassen published his algorithm in 1969. It was the first algorithm to prove that the basic $O(n^3)$ runtime was not optimal.

Suppose $C=AB$. The basic idea behind Strassen's algorithm is to split $A$ and $B$ into 8 submatrices and then recursively compute the submatrices of $C$.

##### 2.4.1 Methodology

Suppose $A, B$ are square and the dimension of $A, B$ are a power of two and $\dim(A)=\dim(B)$.

Then we can represent them as:
$$
A=\begin{bmatrix}A_{11} & A_{12}\\ A_{21} & A_{22} \end{bmatrix}, B=\begin{bmatrix}B_{11} & B_{12}\\ B_{21} & B_{22} \end{bmatrix}
$$
And
$$
C=\begin{bmatrix}C_{11} & C_{12}\\ C_{21} & C_{22} \end{bmatrix}
$$
Then, compute ten matrices $S_1, S_2, \dots, S_{10}$
$$
\begin{aligned}
S_1&=B_{12}-B_{22}\\
S_2&=A_{11}+A_{12}\\
S_3&=A_{21}+A_{22}\\
S_4&=B_{21}-B_{11}\\
S_5&=A_{11}+A_{22}\\
S_6&=B_{11}+B_{22}\\
S_7&=A_{12}-A_{22}\\
S_8&=B_{21}+B_{22}\\
S_9&=A_{11}-A_{21}\\
S_{10}&=B_{11}+B_{12}
\end{aligned}
$$
After that, compute seven matrices $P_1, P_2, \dots, P_7$ recursively
$$
\begin{aligned}
P_1&=A_{11}S_{1}\\
P_2&=S_2B_{22}\\
P_3&=S_3B_{11}\\
P_4&=A_{22}S_4\\
P_5&=S_5S_6\\
P_6&=S_7S_8\\
P_7&=S_9S_{10}
\end{aligned}
$$
And finally
$$
\begin{aligned}
C_{11}&=P_5+P_4-P_2+P_6\\
C_{12}&=P_1+P_2\\
C_{21}&=P_3+P_4\\
C_{22}&=P_5+P_1-P_3-P_7
\end{aligned}
$$

##### 2.4.2 Time Complexity

$$
T(n)=\Theta(n^{\log_27})\approx\Theta(n^{2.807})
$$

Approximately, when $n=296$, **Strassen's algorithm** will give 3x speed acceleration.

![](./images/time_comp.png)

##### 2.4.3 Helper Functions: Matrix Addition and Subtraction

Because we will use matrix addition and subtraction in **Strassen's algorithm**.
$$
\forall i, j, C_{ij}=A_{ij}\pm B_{ij}
$$
**Implementation:**

```c++
matrix add_matrix(matrix m1, matrix m2) {
    if (m1.size() != m2.size() || m1[0].size() != m2[0].size()) {
        cout << "Add error: matrix dimension cannot match."
             << " (" << m1.size() << "*" << m1[0].size() << " + " << m2.size()
             << "*" << m2[0].size() << ")" << endl;
        exit(EXIT_FAILURE);
    }

    matrix res(m1.size(), vector<REAL_NUMBER>(m1[0].size(), 0));

    for (int i = 0; i < m1.size(); i++)
        for (int j = 0; j < m1[0].size(); j++) {
            res[i][j] = m1[i][j] + m2[i][j];
        }

    return res;
}

matrix sub_matrix(matrix m1, matrix m2) {
    if (m1.size() != m2.size() || m1[0].size() != m2[0].size()) {
        cout << "Subtraction error: matrix dimension cannot match."
             << " (" << m1.size() << "*" << m1[0].size() << " - " << m2.size()
             << "*" << m2[0].size() << ")" << endl;
        exit(EXIT_FAILURE);
    }

    matrix res(m1.size(), vector<REAL_NUMBER>(m1[0].size(), 0));

    for (int i = 0; i < m1.size(); i++)
        for (int j = 0; j < m1[0].size(); j++) {
            res[i][j] = m1[i][j] - m2[i][j];
        }

    return res;
}
```

##### 2.4.4 Helper Functions: Matrix Decomposition and Combination

In the first step, we should split the matrix into 4 submatrices.

And in the last step, we should construct $C$ from the 4 submatrices.

Therefore, we need two helper functions to decompose and combine 2-dimension vectors.

For decomposition, use iterator to construct a new vector.

And for combination, use `insert()` to connect two vectors.

**Implementation:**

```c++
// [start, end)
matrix slice_matrix(matrix m, int row_start, int row_end, int col_start,
                    int col_end) {
    matrix res;

    for (int i = row_start; i < row_end; i++) {
        res.push_back(vector<REAL_NUMBER>(m[i].begin() + col_start,
                                          m[i].begin() + col_end));
    }

    return res;
}

matrix merge_matrix(matrix C11, matrix C12, matrix C21, matrix C22) {
    matrix C;

    for (int i = 0; i < C11.size(); i++) {
        C11[i].insert(C11[i].end(), C12[i].begin(), C12[i].end());
    }

    for (int i = 0; i < C21.size(); i++) {
        C21[i].insert(C21[i].end(), C22[i].begin(), C22[i].end());
    }

    C.insert(C.end(), C11.begin(), C11.end());
    C.insert(C.end(), C21.begin(), C21.end());

    return C;
}
```

##### 2.4.5 Helper Function: Free Matrix's Memory

In **Strassen's algorithm**, there are lots of intermediate matrices such as $S_1, S_2, \dots, S_{10}$  and $P_1, P_2, \dots, P_7$.

When $C$ is computed, they are of no use. So we should do garbage collection.

By looking up online, there are two ways to free a matrix's memory:

```c++
void free_matrix(matrix &m) {
    matrix().swap(m);
}
```

Or

```c++
void free_matrix(matrix &m) {
    m.clear();
    m.shrink_to_fit();
}
```

Both are correct.

##### 2.4.6 Implementation

1. Check if the size satisfies the requirement of Strassen's algorithm

   Use `n & (n-1) == 0` to determine whether `n` is a power of two.

2. Termination condition of recursion

   If `N <= STRASSEN_LOWER_BOUND`, use regular matrix multiplication.

   Because when $N$ is small, for-loop is much faster. Details given in part 3.

3. Use the above helper functions to calculate intermediate matrices.

4. Combine $C_{11}, C_{12}, C_{21}, C_{22}$ to get $C$

5. Return $C$

```c++
matrix strassen(matrix A, matrix B) {
    if (A.size() != B.size() || A[0].size() != B[0].size()) {
        cout << "Strassen multiplication error: matrix dimension cannot match."
             << endl;
        exit(EXIT_FAILURE);
    }

    int N = A.size();

    if (N & (N - 1) != 0) {
        cout << "Strassen multiplication error: matrix dimension is not 2^n."
             << endl;
        exit(EXIT_FAILURE);
    }

    if (N <= STRASSEN_LOWER_BOUND) {
        return multiply_matrix(A, B);
    }

    matrix A11 = slice_matrix(A, 0, N / 2, 0, N / 2);
    matrix A12 = slice_matrix(A, 0, N / 2, N / 2, N);
    matrix A21 = slice_matrix(A, N / 2, N, 0, N / 2);
    matrix A22 = slice_matrix(A, N / 2, N, N / 2, N);

    matrix B11 = slice_matrix(B, 0, N / 2, 0, N / 2);
    matrix B12 = slice_matrix(B, 0, N / 2, N / 2, N);
    matrix B21 = slice_matrix(B, N / 2, N, 0, N / 2);
    matrix B22 = slice_matrix(B, N / 2, N, N / 2, N);

    matrix S1 = sub_matrix(B12, B22);
    matrix S2 = add_matrix(A11, A12);
    matrix S3 = add_matrix(A21, A22);
    matrix S4 = sub_matrix(B21, B11);
    matrix S5 = add_matrix(A11, A22);
    matrix S6 = add_matrix(B11, B22);
    matrix S7 = sub_matrix(A12, A22);
    matrix S8 = add_matrix(B21, B22);
    matrix S9 = sub_matrix(A11, A21);
    matrix S10 = add_matrix(B11, B12);

    matrix P1 = strassen(A11, S1);
    matrix P2 = strassen(S2, B22);
    matrix P3 = strassen(S3, B11);
    matrix P4 = strassen(A22, S4);
    matrix P5 = strassen(S5, S6);
    matrix P6 = strassen(S7, S8);
    matrix P7 = strassen(S9, S10);

    matrix C11 = add_matrix(P5, P4);
    C11 = sub_matrix(C11, P2);
    C11 = add_matrix(C11, P6);
    matrix C12 = add_matrix(P1, P2);
    matrix C21 = add_matrix(P3, P4);
    matrix C22 = add_matrix(P5, P1);
    C22 = sub_matrix(C22, P3);
    C22 = sub_matrix(C22, P7);

    matrix C = merge_matrix(C11, C12, C21, C22);

    return C;
}
```

