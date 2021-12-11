#include "cnn.hpp"

int main(int argc, char const* argv[]) {
    Mat image = imread("../samples/face.jpg");

    CNN cnn;
    Mat res = cnn.predict(image);

    cout << res << endl;

    return 0;
}
