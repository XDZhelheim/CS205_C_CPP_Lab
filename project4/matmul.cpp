#include "matrix.hpp"
#include "matrix.cpp"
#include <ctime>

int main(int argc, char const* argv[]) {
    if (argc != 4) {
        printf("Wrong number of arguments.");
        exit(EXIT_FAILURE);
    }
    double start, end;

    start = clock();

    matrix<double> m1 = matrix<double>::read_matrix(argv[1]);
    matrix<double> m2 = matrix<double>::read_matrix(argv[2]);

    end = clock();
    printf("Read file time: %fs\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();

    matrix<double> res = m1 * m2;

    end = clock();
    printf("Multiplication time: %fs\n", (double)(end - start) / CLOCKS_PER_SEC);

    res.print(argv[3]);
    return 0;
}