#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

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
        fscanf(fp, "%f", pread);
        pread++;
    }

    fclose(fp);
    return m;
}

int main(int argc, char const* argv[]) {
    matrix m = read_matrix(
        "H:\\Codes\\C-CppWorkspace\\CS205_C_CPP_Lab\\project3\\data\\test_"
        "matrix.txt");
    printf("%f", m.data[63]);

    return 0;
}
