#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#define DEBUG 0

#define USE_DOUBLE 1

#if USE_DOUBLE
    #define REAL_NUMBER double
    #define PRECISION 15
#else
    #define REAL_NUMBER float
    #define PRECISION 6
#endif

#define STRASSEN_LOWER_BOUND 2

using namespace std;

typedef vector<vector<REAL_NUMBER>> matrix;

matrix read_matrix(const char *file_name) {
    ifstream in(file_name);
    if (!in.is_open()) {
        cout << "Error opening file." << endl;
        exit(EXIT_FAILURE);
    }

    matrix m;

    REAL_NUMBER temp;
    int i = 0, j = 0;
    m.push_back(vector<REAL_NUMBER>());
    while (in >> temp) {
        m[i].push_back(temp);
        if (in.get() == '\n' && in.peek() != EOF) {
            m.push_back(vector<REAL_NUMBER>());
            i++;
            j = 0;
        }
    }

    in.close();
    return m;
}

void print_matrix(matrix m, const char *file_name) {
    ofstream out(file_name);
    out.precision(PRECISION);

    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].size(); j++) {
            out << m[i][j] << " ";
        }
        out << endl;
    }

    out.close();
}

void print_matrix(matrix m) {
    cout.precision(PRECISION);

    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].size(); j++) {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
}

matrix multiply_matrix(matrix m1, matrix m2) {
    // a*b dot b*c = a*c
    int nrows = m1.size();
    int ncols = m2[0].size();
    int intermediate = m2.size();

    if (intermediate != m1[0].size()) {
        cout << "Multiplication error: matrix dimension cannot match."
             << " (" << m1.size() << "*" << m1[0].size() << " dot " << m2.size()
             << "*" << m2[0].size() << ")" << endl;
        exit(EXIT_FAILURE);
    }

    matrix product(nrows, vector<REAL_NUMBER>(ncols, 0));

    for (int i = 0; i < nrows; i++)
        for (int k = 0; k < intermediate; k++)
            for (int j = 0; j < ncols; j++) {
                product[i][j] += m1[i][k] * m2[k][j];
            }

    return product;
}

matrix add_matrix(matrix m1, matrix m2) {
    if (m1.size() != m2.size() || m1[0].size() != m2[0].size()) {
        cout << "Add error: matrix dimension cannot match."
             << " (" << m1.size() << "*" << m1[0].size() << " + " << m2.size()
             << "*" << m2[0].size() << ")" << endl;
        exit(EXIT_FAILURE);
    }

    matrix res(m1.size(), vector<REAL_NUMBER>(m1[0].size(), 0));

    for (int i = 0; i < m1.size(); i++)
        for (int j = 0; j < m1[0].size(); j++) {
            res[i][j] = m1[i][j] + m2[i][j];
        }

    return res;
}

matrix sub_matrix(matrix m1, matrix m2) {
    if (m1.size() != m2.size() || m1[0].size() != m2[0].size()) {
        cout << "Subtraction error: matrix dimension cannot match."
             << " (" << m1.size() << "*" << m1[0].size() << " - " << m2.size()
             << "*" << m2[0].size() << ")" << endl;
        exit(EXIT_FAILURE);
    }

    matrix res(m1.size(), vector<REAL_NUMBER>(m1[0].size(), 0));

    for (int i = 0; i < m1.size(); i++)
        for (int j = 0; j < m1[0].size(); j++) {
            res[i][j] = m1[i][j] - m2[i][j];
        }

    return res;
}

// [start, end)
matrix slice_matrix(matrix m, int row_start, int row_end, int col_start,
                    int col_end) {
    matrix res;

    for (int i = row_start; i < row_end; i++) {
        res.push_back(vector<REAL_NUMBER>(m[i].begin() + col_start,
                                          m[i].begin() + col_end));
    }

    return res;
}

matrix merge_matrix(matrix C11, matrix C12, matrix C21, matrix C22) {
    matrix C;

    for (int i = 0; i < C11.size(); i++) {
        C11[i].insert(C11[i].end(), C12[i].begin(), C12[i].end());
    }

    for (int i = 0; i < C21.size(); i++) {
        C21[i].insert(C21[i].end(), C22[i].begin(), C22[i].end());
    }

    C.insert(C.end(), C11.begin(), C11.end());
    C.insert(C.end(), C21.begin(), C21.end());

    return C;
}

void free_matrix(matrix &m) {
    // m.clear();
    // m.shrink_to_fit();
    matrix().swap(m);
}

matrix strassen(matrix A, matrix B) {
    if (A.size() != B.size() || A[0].size() != B[0].size()) {
        cout << "Strassen multiplication error: matrix dimension cannot match."
             << endl;
        exit(EXIT_FAILURE);
    }

    int N = A.size();

    if (N & (N - 1) != 0) {
        cout << "Strassen multiplication error: matrix dimension is not 2^n."
             << endl;
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

    matrix S1 = sub_matrix(B12, B22);
    matrix S2 = add_matrix(A11, A12);
    matrix S3 = add_matrix(A21, A22);
    matrix S4 = sub_matrix(B21, B11);
    matrix S5 = add_matrix(A11, A22);
    matrix S6 = add_matrix(B11, B22);
    matrix S7 = sub_matrix(A12, A22);
    matrix S8 = add_matrix(B21, B22);
    matrix S9 = sub_matrix(A11, A21);
    matrix S10 = add_matrix(B11, B12);

    matrix P1 = strassen(A11, S1);
    matrix P2 = strassen(S2, B22);
    matrix P3 = strassen(S3, B11);
    matrix P4 = strassen(A22, S4);
    matrix P5 = strassen(S5, S6);
    matrix P6 = strassen(S7, S8);
    matrix P7 = strassen(S9, S10);

    matrix C11 = add_matrix(P5, P4);
    C11 = sub_matrix(C11, P2);
    C11 = add_matrix(C11, P6);
    matrix C12 = add_matrix(P1, P2);
    matrix C21 = add_matrix(P3, P4);
    matrix C22 = add_matrix(P5, P1);
    C22 = sub_matrix(C22, P3);
    C22 = sub_matrix(C22, P7);

    matrix C = merge_matrix(C11, C12, C21, C22);

    free_matrix(A11);
    free_matrix(A12);
    free_matrix(A21);
    free_matrix(A22);
    free_matrix(B11);
    free_matrix(B12);
    free_matrix(B21);
    free_matrix(B22);
    free_matrix(S1);
    free_matrix(S2);
    free_matrix(S3);
    free_matrix(S4);
    free_matrix(S5);
    free_matrix(S6);
    free_matrix(S7);
    free_matrix(S8);
    free_matrix(S9);
    free_matrix(S10);
    free_matrix(P1);
    free_matrix(P2);
    free_matrix(P3);
    free_matrix(P4);
    free_matrix(P5);
    free_matrix(P6);
    free_matrix(P7);
    free_matrix(C11);
    free_matrix(C12);
    free_matrix(C21);
    free_matrix(C22);

    return C;
}

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        cout << "Wrong number of arguments.";
        exit(EXIT_FAILURE);
    }
    double start, end;

    start = clock();

    matrix m1 = read_matrix(argv[1]);
    matrix m2 = read_matrix(argv[2]);

    end = clock();
    cout << "Read file time: " << (end - start) / (double)1000 << "s" << endl;

    start = clock();

    matrix res = strassen(m1, m2);

    end = clock();
    cout << "Multiplication time: " << (end - start) / (double)1000 << "s" << endl;

    print_matrix(res, argv[3]);

    return 0;
}
