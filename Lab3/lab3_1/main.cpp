#include <iostream>

#include "fib.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    int n = 0;
    while (n <= 0) {
        cout << "Please input a positive integer" << endl;
        cin >> n;
    }

    for (int i = 1; i <= n; i++) {
        cout << fib(i) << " ";
        if (i % 10 == 0) cout << endl;
    }
    cout << endl;
    return 0;
}
