#define STRASSEN_LOWER_BOUND 128

typedef float REAL_NUMBER;
typedef struct
{
    int nrows;
    int ncols;
    REAL_NUMBER* data;
} matrix;

matrix read_matrix(const char *file_name);

void print_matrix_stdout(matrix m);
void print_matrix(matrix m, const char *file_name);

matrix multiply_matrix(matrix m1, matrix m2);
matrix add_matrix(matrix m1, matrix m2);
matrix sub_matrix(matrix m1, matrix m2);

matrix submatrix(matrix m, int row_start, int row_end, int col_start, int col_end);
matrix merge_matrix(matrix C11, matrix C12, matrix C21, matrix C22);

matrix strassen(matrix A, matrix B);

void copy_matrix(matrix dst, matrix src);