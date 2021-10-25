#include <iostream>

using namespace std;

template <typename T>
inline T minimum(T x, T y) {
    return x < y ? x : y;
}

int main(int argc, char const* argv[]) {
    cout << minimum(1, 2) << endl;
    cout << minimum('a', 'b') << endl;
    cout << minimum(1.1f, 1.2f) << endl;
    return 0;
}
