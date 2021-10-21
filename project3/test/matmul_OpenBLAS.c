#include <cblas.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define STRASSEN_LOWER_BOUND 128
#define NAN -1

typedef float REAL_NUMBER;
typedef struct
{
    int nrows;
    int ncols;
    REAL_NUMBER* data;
} matrix;

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

matrix create_matrix(int nrows, int ncols, REAL_NUMBER fill) {
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

int main(int argc, char const* argv[]) {
    if (argc != 4) {
        printf("Wrong number of arguments.");
        exit(EXIT_FAILURE);
    }

    const enum CBLAS_ORDER Order = CblasRowMajor;
    const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
    const enum CBLAS_TRANSPOSE TransB = CblasNoTrans;
    const float alpha=1;
    const float beta=0;

    double start, end;

    start = clock();

    matrix m1 = read_matrix(argv[1]);
    matrix m2 = read_matrix(argv[2]);

    end = clock();
    printf("Read file time: %fs\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();

    matrix res = create_matrix(m1.nrows, m2.ncols, NAN);
    cblas_sgemm(Order, TransA, TransB, m1.nrows, m2.ncols, m1.ncols, alpha, m1.data, m1.ncols, m2.data, m2.ncols, beta, res.data, res.ncols);

    end = clock();
    printf("Multiplication time: %fs\n", (double)(end - start) / CLOCKS_PER_SEC);

    print_matrix(&res, argv[3]);
    return 0;
}

// gcc -I D:\OpenBLAS-0.3.18-x64\include -L D:\OpenBLAS-0.3.18-x64\lib .\matmul_OpenBLAS.c -lopenblas -o matmul_OpenBLAS