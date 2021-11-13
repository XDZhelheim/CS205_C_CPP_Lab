#include "matrix.hpp"

template <typename T>
inline matrix<T>::matrix(int nrows, int ncols, T fill) {
    this->nrows = nrows;
    this->ncols = ncols;
    this->data = new T[nrows * ncols];

    if (fill != NAN) {
        for (int i = 0; i < nrows * ncols; i++) {
            m.data[i] = fill;
        }
    }
}

template <typename T>
inline matrix<T>::~matrix() {
    if (this->data != nullptr) {
        this->nrows = 0;
        this->ncols = 0;
        delete[] this->data;
        this->data = nullptr;
    }
}

template <typename T>
matrix<T> matrix<T>::read_matrix(const char* file_name) {
    FILE* fp = fopen(file_name, "r");
    matrix<T> m(0, 0, NAN);

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
    T* pread = m.data;
    for (int i = 0; i < m.ncols * m.nrows; i++) {
        // fscanf(fp, "%f", &m.data[i]);
        // fscanf(fp, "%f", m.data + i);
        fscanf(fp, "%f", pread);
        pread++;
    }

    fclose(fp);
    return m;
}

template <typename T>
void matrix<T>::print(const char* file_name) {
    FILE* fp = fopen(file_name, "w");

    for (int i = 0; i < this->ncols * this->nrows; i++) {
        if (i > 0 && i % this->ncols == 0) {
            fputc('\n', fp);
        }
        fprintf(fp, "%g ", this->data[i]);
    }
    fputc('\n', fp);  // last blank line

    fclose(fp);
}

template <typename T>
inline T* matrix<T>::operator[](int i) {
    return this->data[i * this->ncols];
}

template <typename T>
inline matrix<T> matrix<T>::operator+(const matrix& other) {
    if (this->nrows != other->nrows || this->ncols != other->ncols) {
        printf("Addition error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    matrix<T> res(this->nrows, this->ncols, NAN);

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] = this->data[i] + other->data[i];
    }

    return res;
}

template <typename T>
inline matrix<T> matrix<T>::operator-(const matrix& other) {
    if (this->nrows != other->nrows || this->ncols != other->ncols) {
        printf("Subtraction error: matrix dimension cannot match.\n");
        exit(EXIT_FAILURE);
    }

    matrix<T> res(this->nrows, this->ncols, NAN);

    for (int i = 0; i < res.nrows * res.ncols; i++) {
        res.data[i] = this->data[i] - other->data[i];
    }

    return res;
}
