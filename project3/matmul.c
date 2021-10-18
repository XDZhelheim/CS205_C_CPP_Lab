#include "matrix.h"

int main(int argc, char const *argv[])
{
    if (argc != 4) {
        printf("Wrong number of arguments.");
        exit(EXIT_FAILURE);
    }
    double start, end;

    start = clock();

    matrix m1 = read_matrix(argv[1]);
    matrix m2 = read_matrix(argv[2]);

    end = clock();
    printf("Read file time: %fs\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();

    matrix res;
    res = strassen(&m1, &m2);

    end = clock();
    printf("Multiplication time: %fs\n", (double)(end - start) / CLOCKS_PER_SEC);

    print_matrix(&res, argv[3]);
    return 0;
}
