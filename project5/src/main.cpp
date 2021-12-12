#include "cnn.hpp"
#include "cnn.cpp"

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char const* argv[]) {

    CNN cnn;
    Matrix2dArray<float> res = cnn.predict("../samples/face.jpg");

    res.print();

    return 0;
}
