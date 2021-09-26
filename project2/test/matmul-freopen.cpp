#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>

#define DEBUG 0

#define USE_DOUBLE 0

#if USE_DOUBLE
    #define REAL_NUMBER double
#else
    #define REAL_NUMBER float
#endif

using namespace std;

typedef vector<vector<REAL_NUMBER>> matrix;

matrix read_matrix(const char *file_name) {
    if (!freopen(file_name, "r", stdin)) {
        cout << "Error opening file." << endl;
        exit(EXIT_FAILURE);
    }

    matrix m;

    REAL_NUMBER temp;
    int i = 0, j = 0;
    m.push_back(vector<REAL_NUMBER>());
    while (cin >> temp) {
        m[i].push_back(temp);
        if (cin.get() == '\n' && cin.peek() != EOF) {
            m.push_back(vector<REAL_NUMBER>());
            i++;
            j = 0;
        }
    }

    cin.clear(); // reset the status bits of cin
    return m;
}

void print_matrix(matrix m) {
    for (int i = 0; i < m.size(); i++) {
        for (int j = 0; j < m[0].size(); j++) {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
}

void print_matrix(matrix m, const char *file_name) {
    freopen(file_name, "w", stdout);

    print_matrix(m);
}

matrix multiply_matrix(matrix m1, matrix m2) {
    // a*b dot b*c = a*c
    int nrows = m1.size();
    int ncols = m2[0].size();
    int intermediate = m2.size();

    if (intermediate != m1[0].size()) {
        cout << "Wrong matrix dimension." << endl;
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

int main(int argc, char const *argv[]) {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    if (argc != 4) {
        cout << "Wrong number of arguments.";
        exit(EXIT_FAILURE);
    }
    double start, end;

    start=clock();

    matrix m1 = read_matrix(argv[1]);
    matrix m2 = read_matrix(argv[2]);

    end=clock();
    cout<<"Read file time: "<<(end-start)/(double)1000<<"s"<<endl;

    start=clock();

    matrix res = multiply_matrix(m1, m2);

    end=clock();
    cout<<"Multiplication time: "<<(end-start)/(double)1000<<"s"<<endl;

    print_matrix(res, argv[3]);

    if (DEBUG) {
        cout << m1.size() << "*" << m1[0].size() << " dot " << m2.size() << "*"
             << m2[0].size() << " = " << res.size() << "*" << res[0].size() << endl;
    }

    return 0;
}
