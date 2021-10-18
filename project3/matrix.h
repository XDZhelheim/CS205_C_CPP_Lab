#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STRASSEN_LOWER_BOUND 128

typedef float REAL_NUMBER;
typedef struct
{
    int nrows;
    int ncols;
    REAL_NUMBER* data;
} matrix;

matrix read_matrix(const char *file_name);
void delete_matrix(matrix* m);

REAL_NUMBER get_elem(matrix* m, int i, int j);
REAL_NUMBER* get_elem_ptr(matrix* m, int i, int j);
void set_elem(matrix* m, int i, int j, REAL_NUMBER num);

void print_matrix(matrix* m, const char *file_name);
void print_matrix_stdout(matrix* m);

matrix multiply_matrix(matrix* m1, matrix* m2);
matrix add_matrix(matrix* m1, matrix* m2);
matrix sub_matrix(matrix* m1, matrix* m2);
void copy_matrix(matrix* dst, matrix* src);

matrix slice_matrix(matrix* m, int row_start, int row_end, int col_start, int col_end);
matrix merge_matrix(matrix* C11, matrix* C12, matrix* C21, matrix* C22);

matrix strassen(matrix* A, matrix* B);