#include <iostream>

using namespace std;

template <typename T>
class Matrix {
   public:
    int nrows;
    int ncols;
    T* data;
    int* ref_count;

    Matrix(int nrows, int ncols, T fill) {
        this->nrows = nrows;
        this->ncols = ncols;
        this->data = new T[nrows * ncols];

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

    Matrix<T>& operator=(const Matrix<T>& other) {
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

    T* operator[](int i) {
        if (i > this->nrows) {
            cout << "Index error: array index out of bound." << endl;
            exit(EXIT_FAILURE);
        }

        return this->data + i * this->ncols;
    }

    T& operator()(int i, int j) {
        if (i > this->nrows - 1 || j > this->ncols - 1) {
            cout << "Index error: array index out of bound." << endl;
            exit(EXIT_FAILURE);
        }

        return this->data[i * this->ncols + j];
    }

    T operator()(int i, int j) const {
        if (i > this->nrows - 1 || j > this->ncols - 1) {
            cout << "Index error: array index out of bound." << endl;
            exit(EXIT_FAILURE);
        }

        return this->data[i * this->ncols + j];
    }

    Matrix<T> operator+(Matrix<T>& other) {
        if (this->nrows != other.nrows || this->ncols != other.ncols) {
            cout << "Addition error: Matrix dimension cannot match." << endl;
            exit(EXIT_FAILURE);
        }

        Matrix<T> res(this->nrows, this->ncols, 0);

        for (int i = 0; i < res.nrows * res.ncols; i++) {
            res.data[i] = this->data[i] + other.data[i];
        }

        return res;
    }

    Matrix<T>& operator+=(Matrix<T>& other) {
        *this = (*this) + other;
        return *this;
    }

    Matrix<T> operator*(T f) {
        Matrix<T> res(this->nrows, this->ncols, 0);

        for (int i = 0; i < res.nrows * res.ncols; i++) {
            res.data[i] = this->data[i] * f;
        }

        return res;
    }

    Matrix<T> operator*(Matrix<T>& other) {
        if (this->ncols != other.nrows) {
            cout << "Multiplication error: Matrix dimension cannot match." << endl;
            exit(EXIT_FAILURE);
        }

        Matrix<T> res(this->nrows, other.ncols, 0);

        for (int i = 0; i < this->nrows; i++)
            for (int k = 0; k < this->ncols; k++)
                for (int j = 0; j < other.ncols; j++) {
                    res[i][j] += (*this)[i][k] * other[k][j];
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
    Matrix<float> a(3, 4, 0.5f);
    Matrix<float> b(3, 4, 3.0f);
    Matrix<float> c = a + b;
    a += b;
    Matrix<float> d(4, 3, 1.0f);

    c.print();
    cout << endl;
    cout << a(2, 3) << endl;
    (a * d).print();
    return 0;
}
