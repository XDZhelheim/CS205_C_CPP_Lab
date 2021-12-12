#include "cnn.hpp"
#include "params.hpp"

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char const* argv[]) {
    // CNN cnn;
    // Matrix2dArray<float> res = cnn.predict("../samples/face.jpg");

    // res.print();

    int a[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int b[6] = {1, 2, 3, 4, 5, 6};
    int c[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    int d[16] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};

    int k[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    int k2[4] = {3, 3, 3, 3};
    Matrix<int> kernel(3, 3, k);
    Matrix<int> kernel2(2, 2, k2);

    // Matrix<int> m1(3, 3, a);
    // m1.pad(2).print();
    // cout << endl;

    // Matrix<int> m2(2, 3, b);
    // m2.pad(1).print();
    // cout << endl;
    // m2.pad(2).print();

    // Matrix<int> m3(4, 4, c);
    // m3.pad(1).print();
    // cout << endl;
    // m3.pad(2).print();
    // cout << endl;
    // m3.pad(3).print();

    Matrix<int> m4(4, 4, d);
    Matrix<int> m4_pad = m4.pad((3 - 1) / 2);

    int size = 4;
    int kernel_size = 2;
    int padding = 0;
    int stride = 2;

    // int pad_matrix_size = 4;
    // for (int i = 0; i <= pad_matrix_size - kernel_size; i += stride) {
    //     for (int j = 0; j <= pad_matrix_size - kernel_size; j += stride) {
    //         m4.submatrix_ROI(i, i + kernel_size, j, j + kernel_size).multiply_elements(kernel2).print();
    //         cout << endl;
    //     }
    // }

    m4.convolution(kernel, padding, stride).print();
    cout << endl;
    m4.convolution(kernel2, padding, stride).print();

    return 0;
}
