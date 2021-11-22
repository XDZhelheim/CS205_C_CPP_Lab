#include <iostream>

using namespace std;

class Matrix {
   public:
    int nrows;
    int ncols;
    float* data;
    int* ref_count;

    Matrix(int nrows, int ncols, float fill) {
        this->nrows = nrows;
        this->ncols = ncols;
        this->data = new float[nrows * ncols];

        this->ref_count = new int(1);

        for (int i = 0; i < nrows * ncols; i++) {
            this->data[i] = fill;
        }
    }

    Matrix(const Matrix& other) {
        this->nrows = other.nrows;
        this->ncols = other.ncols;

        this->data = other.data;
        this->ref_count = other.ref_count;
        *(this->ref_count) += 1;
    }

    ~Matrix() {
        *(this->ref_count) -= 1;
        if (*(this->ref_count) == 0 && this->data != nullptr) {
            delete[] this->data;
            delete this->ref_count;
        }
    }

    Matrix& operator=(const Matrix& other) {
        this->nrows = other.nrows;
        this->ncols = other.ncols;

        *(this->ref_count) -= 1;
        if (*(this->ref_count) == 0 && this->data != nullptr) {
            delete this->ref_count;
            delete[] this->data;
        }

        this->data = other.data;
        this->ref_count = other.ref_count;
        *(this->ref_count) += 1;

        return *this;
    }

    float* operator[](int i) {
        if (i > this->nrows) {
            cout << "Index error: array index out of bound." << endl;
            exit(EXIT_FAILURE);
        }

        return this->data + i * this->ncols;
    }

    Matrix operator+(Matrix& other) {
        if (this->nrows != other.nrows || this->ncols != other.ncols) {
            cout << "Addition error: matrix dimension cannot match." << endl;
            exit(EXIT_FAILURE);
        }

        Matrix res(this->nrows, this->ncols, 0);

        for (int i = 0; i < res.nrows * res.ncols; i++) {
            res.data[i] = this->data[i] + other.data[i];
        }

        return res;
    }

    Matrix operator*(float f) {
        Matrix res(this->nrows, this->ncols, 0);

        for (int i = 0; i < res.nrows * res.ncols; i++) {
            res.data[i] = this->data[i] * f;
        }

        return res;
    }

    void print() {
        for (int i = 0; i < this->nrows; i++) {
            for (int j = 0; j < this->ncols; j++) {
                cout << (*this)[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main(int argc, char const* argv[]) {
    Matrix a(3, 4, 0.5f);
    Matrix b(3, 4, 3.0f);
    Matrix c = a + b;
    Matrix d = a * 2.0f;

    c.print();
    cout << endl;
    d.print();
    return 0;
}
