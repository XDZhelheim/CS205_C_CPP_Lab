#include <iostream>

using namespace std;

template <typename T>
inline T abso(T x) {
    return x < 0 ? -x : x;
}

int vabs(int* p, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += abso(p[i]);
    }
    return sum;
}

float vabs(float* p, int n) {
    float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += abso(p[i]);
    }
    return sum;
}

double vabs(double* p, int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += abso(p[i]);
    }
    return sum;
}

int main(int argc, char const* argv[]) {
    int n = 5;

    int arri[n] = {-1, 2, -3, 4, -5};
    float arrf[n] = {-1.0f, 2.0f, -3.0f, 4.0f, -5.0f};
    double arrd[n] = {-1.0, 2.0, -3.0, 4.0, -5.0};

    cout << vabs(arri, n) << endl;
    cout << vabs(arrf, n) << endl;
    cout << vabs(arrd, n) << endl;

    return 0;
}
