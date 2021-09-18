#include <iostream>

#include "fac.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    int n = 0;
    while (n <= 0) {
        cout << "Please input a positive integer" << endl;
        cin >> n;
    }

    for (int i = 1; i <= n; i++) 
        cout << i << "! = " << fac(i) << endl;
    return 0;
}
