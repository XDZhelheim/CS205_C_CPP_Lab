#include "matrix.h"

matrix read_matrix(const char* file_name) {
    FILE* fp = fopen(file_name, "r");
    matrix m;
    m.nrows = 0;
    m.ncols = 0;

    // get ncols and nrows
    REAL_NUMBER tempf = 0;
    char tempc = 1;
    while (tempc != '\n') {
        fscanf(fp, "%f", &tempf);
        fscanf(fp, "%c", &tempc);
        m.ncols++;
    }
    m.nrows++;
    while (fscanf(fp, "%c", &tempc) != EOF) {
        if (tempc == '\n') {
            m.nrows++;
        }
    }

    // read data
    rewind(fp);
    m.data = (REAL_NUMBER*)malloc(m.ncols * m.nrows * sizeof(REAL_NUMBER));
    REAL_NUMBER* pread = m.data;
    for (int i = 0; i < m.ncols * m.nrows; i++) {
        // fscanf(fp, "%f", &m.data[i]);
        // fscanf(fp, "%f", m.data + i);
        fscanf(fp, "%f", pread);
        pread++;
    }

    fclose(fp);
    return m;
}

inline matrix create_matrix(int nrows, int ncols, REAL_NUMBER fill) {
    matrix m = {
        .nrows = nrows,
        .ncols = ncols,
        .data = (REAL_NUMBER*)malloc(nrows * ncols * sizeof(REAL_NUMBER))};

    if (fill != NAN) {
        for (int i = 0; i < nrows * ncols; i++) {
            m.data[i] = fill;
        }
    }

    return m;
}

inline void delete_matrix(matrix* m) {
    m->nrows = 0;
    m->ncols = 0;
    free(m->data);
    m->data = NULL;
}

inline REAL_NUMBER get_elem(matrix* m, int i, int j) {
    return m->data[i * m->ncols + j];
}

inline REAL_NUMBER* get_elem_ptr(matrix* m, int i, int j) {
    return m->data + (i * m->ncols + j);
}

inline void set_elem(matrix* m, int i, int j, REAL_NUMBER num) {
    m->data[i * m->ncols + j] = num;
}

void print_matrix(matrix* m, const char* file_name) {
    FILE* fp = fopen(file_name, "w");

    for (int i = 0; i < m->ncols * m->nrows; i++) {
        if (i > 0 && i % m->ncols == 0) {
            fputc('\n', fp);
        }
        fprintf(fp, "%g ", m->data[i]);
    }
    fputc('\n', fp);  // last blank line

    fclose(fp);
}

void print_matrix_stdout(matrix* m) {
    for (int i = 0; i < m->ncols * m->nrows; i++) {
        if (i > 0 && i % m->ncols == 0) {
            putchar('\n');
        }
        printf("%g ", m->data[i]);
    }
}

matrix multiply_matrix(matrix* m1, matrix* m2) {
    if (m1->ncols != m2->nrows) {
        printf("Multiplication error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    matrix res = create_matrix(m1->nrows, m2->ncols, 0);

    for (int i = 0; i < m1->nrows; i++)
        for (int k = 0; k < m1->ncols; k++)
            for (int j = 0; j < m2->ncols; j++) {
                *get_elem_ptr(&res, i, j) += get_elem(m1, i, k) * get_elem(m2, k, j);
            }

    return res;
}

matrix add_matrix(matrix* m1, matrix* m2) {
    if (m1->nrows != m2->nrows || m1->ncols != m2->ncols) {
        printf("Addition error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    matrix res = create_matrix(m1->nrows, m1->ncols, NAN);

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] = m1->data[i] + m2->data[i];
    }

    return res;
}

matrix sub_matrix(matrix* m1, matrix* m2) {
    if (m1->nrows != m2->nrows || m1->ncols != m2->ncols) {
        printf("Subtraction error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    matrix res = create_matrix(m1->nrows, m1->ncols, NAN);

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] = m1->data[i] - m2->data[i];
    }

    return res;
}

void copy_matrix(matrix* dst, matrix* src) {
    if (dst->nrows != dst->nrows || src->ncols != src->ncols) {
        printf("Copy error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < src->nrows * src->ncols; i++) {
        dst->data[i] = src->data[i];
    }
}

matrix slice_matrix(matrix* m, int row_start, int row_end, int col_start, int col_end) {
    matrix res = create_matrix(row_end - row_start, col_end - col_start, NAN);

    for (int i = row_start; i < row_end; i++)
        for (int j = col_start; j < col_end; j++) {
            set_elem(&res, i - row_start, j - col_start, get_elem(m, i, j));
        }

    return res;
}

matrix merge_matrix(matrix* C11, matrix* C12, matrix* C21, matrix* C22) {
    matrix C = create_matrix(C11->nrows + C21->nrows, C11->ncols + C12->ncols, NAN);

    for (int i = 0; i < C11->nrows; i++)
        for (int j = 0; j < C11->ncols; j++) {
            set_elem(&C, i, j, get_elem(C11, i, j));
        }
    for (int i = 0; i < C12->nrows; i++)
        for (int j = 0; j < C12->ncols; j++) {
            set_elem(&C, i, j + C11->ncols, get_elem(C12, i, j));
        }
    for (int i = 0; i < C21->nrows; i++)
        for (int j = 0; j < C21->ncols; j++) {
            set_elem(&C, i + C11->nrows, j, get_elem(C21, i, j));
        }
    for (int i = 0; i < C22->nrows; i++)
        for (int j = 0; j < C22->ncols; j++) {
            set_elem(&C, i + C11->nrows, j + C11->ncols, get_elem(C22, i, j));
        }

    return C;
}

matrix strassen(matrix* A, matrix* B) {
    if (A->nrows != B->nrows || A->ncols != B->ncols) {
        printf("Strassen multiplication error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    int N = A->nrows;

    if ((N & (N - 1)) != 0) {
        printf("Strassen multiplication error: matrix dimension is not 2^n.\n");
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

    matrix S1 = sub_matrix(&B12, &B22);
    matrix S2 = add_matrix(&A11, &A12);
    matrix S3 = add_matrix(&A21, &A22);
    matrix S4 = sub_matrix(&B21, &B11);
    matrix S5 = add_matrix(&A11, &A22);
    matrix S6 = add_matrix(&B11, &B22);
    matrix S7 = sub_matrix(&A12, &A22);
    matrix S8 = add_matrix(&B21, &B22);
    matrix S9 = sub_matrix(&A11, &A21);
    matrix S10 = add_matrix(&B11, &B12);

    matrix P1 = strassen(&A11, &S1);
    matrix P2 = strassen(&S2, &B22);
    matrix P3 = strassen(&S3, &B11);
    matrix P4 = strassen(&A22, &S4);
    matrix P5 = strassen(&S5, &S6);
    matrix P6 = strassen(&S7, &S8);
    matrix P7 = strassen(&S9, &S10);

    matrix C11 = add_matrix(&P5, &P4);
    C11 = sub_matrix(&C11, &P2);
    C11 = add_matrix(&C11, &P6);
    matrix C12 = add_matrix(&P1, &P2);
    matrix C21 = add_matrix(&P3, &P4);
    matrix C22 = add_matrix(&P5, &P1);
    C22 = sub_matrix(&C22, &P3);
    C22 = sub_matrix(&C22, &P7);

    matrix C = merge_matrix(&C11, &C12, &C21, &C22);

    delete_matrix(&A11); delete_matrix(&A12); delete_matrix(&A21); delete_matrix(&A22);
    delete_matrix(&B11); delete_matrix(&B12); delete_matrix(&B21); delete_matrix(&B22);
    delete_matrix(&C11); delete_matrix(&C12); delete_matrix(&C21); delete_matrix(&C22);
    delete_matrix(&S1); delete_matrix(&S2); delete_matrix(&S3); delete_matrix(&S4); delete_matrix(&S5);
    delete_matrix(&S6); delete_matrix(&S7); delete_matrix(&S8); delete_matrix(&S9); delete_matrix(&S10);
    delete_matrix(&P1); delete_matrix(&P2); delete_matrix(&P3); delete_matrix(&P4);
    delete_matrix(&P5); delete_matrix(&P6); delete_matrix(&P7);

    return C;
}

// int main(int argc, char const* argv[]) {
//     matrix m = read_matrix("H:\\Codes\\C-CppWorkspace\\CS205_C_CPP_Lab\\project3\\data\\test_matrix.txt");
//     matrix m2 = read_matrix("H:\\Codes\\C-CppWorkspace\\CS205_C_CPP_Lab\\project3\\data\\test_matrix.txt");
//     matrix m3 = read_matrix("H:\\Codes\\C-CppWorkspace\\CS205_C_CPP_Lab\\project3\\data\\test_matrix.txt");
//     matrix m4 = read_matrix("H:\\Codes\\C-CppWorkspace\\CS205_C_CPP_Lab\\project3\\data\\test_matrix.txt");
//     // printf("%f", get_elem(m, 1, 0));
//     // print_matrix(m, "./temp.txt");
//     // printf("%p\n", m.data);
//     // delete_matrix(&m);
//     // printf("%p\n", m.data);
//     // set_elem(&m, 2, 3, 666);
//     // printf("%f\n", get_elem(&m, 2, 3));
//     matrix merged=merge_matrix(&m, &m2, &m3, &m4);
//     print_matrix_stdout(&merged);

//     return 0;
// }
