#include <cstring>
#include <iostream>

using namespace std;

struct MyMatrix {
    float* mat;  // array of floats.
    size_t row;  // number of rows in the matrix.
    size_t col;  // number of columns in the matrix.

    float& operator()(int r, int c) {
        return mat[r * col + c];
    }

    size_t length() {
        return row * col;
    }
};

MyMatrix allocMatrix(int row, int col) {
    struct MyMatrix result;
    result.row = row;
    result.col = col;
    result.mat = (float*)malloc(sizeof(float) * row * col);

    return result;
}

void freeMatrix(struct MyMatrix& matrix) {
    free(matrix.mat);
}

MyMatrix readMatFromFile(const char* filename, int row, int col) {
    FILE* file = fopen(filename, "r");
    MyMatrix result = allocMatrix(row, col);
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            fscanf(file, "%f", &result(i, j));
    fclose(file);
    return result;
}

void writeMatToFile(MyMatrix& mat, const char* filename) {
    FILE* file = fopen(filename, "w");
    fprintf(file, "%zu %zu\n", mat.row, mat.col);
    for (int i = 0; i < mat.row; ++i) {
        for (int j = 0; j < mat.col; ++j)
            fprintf(file, " %f", mat(i, j));
        fprintf(file, "\n");
    }
    fclose(file);
}

MyMatrix multiply(MyMatrix& a, MyMatrix& b) {
    MyMatrix result = allocMatrix(a.row, b.col);
    memset(result.mat, 0, sizeof(float) * result.length());
    if (a.col != b.row)
        return result;
    for (int i = 0; i < a.row; i++)
        for (int k = 0; k < a.col; k++)
            for (int j = 0; j < b.col; j++)
                result.mat[i * b.col + j] += a.mat[i * a.col + k] * b.mat[k * b.col + j];
    return result;
}

MyMatrix add_row(MyMatrix& a, MyMatrix& b) {
    MyMatrix result = allocMatrix(a.row, a.col);
    if (a.row != b.row || a.col != b.col)
        return result;
    for (int i = 0; i < a.row; i++)
        for (int j = 0; j < a.col; j++)
            result(i, j) = a(i, j) + b(i, j);
    return result;
}

MyMatrix add_col(MyMatrix& a, MyMatrix& b) {
    MyMatrix result = allocMatrix(a.row, a.col);
    if (a.row != b.row || a.col != b.col)
        return result;
    for (int j = 0; j < a.col; j++)
        for (int i = 0; i < a.row; i++)
            result(i, j) = a(i, j) + b(i, j);
    return result;
}

int main() {
    clock_t t1 = clock();
    MyMatrix mat1 = readMatFromFile("./mat1.txt", 10000, 1000);
    MyMatrix mat2 = readMatFromFile("./mat2.txt", 10000, 1000);
    clock_t t2 = clock();
    printf("Read file used %lf second(s).\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

    t1 = clock();
    MyMatrix res = add_row(mat1, mat2);
    t2 = clock();
    printf("Addition (row) used %lf second(s).\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

    t1 = clock();
    res = add_col(mat1, mat2);
    t2 = clock();
    printf("Addition (col) used %lf second(s).\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

    // writeMatToFile(res, "2048_3.txt");

    freeMatrix(mat1);
    freeMatrix(mat2);
    freeMatrix(res);
    return 0;
}