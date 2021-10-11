#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    int n = 5;
    int *arr = new int[n]{1, 2, 3, 4, 5};

    int *p = arr + n - 1;
    for (int i = 0; i < n; i++) {
        cout << *p << " ";
        p--;
    }
    cout << endl;

    delete[] arr;
    return 0;
}
