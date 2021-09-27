#include "stuinfo.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    int n = 5;
    stuinfo stu[n];

    inputstu(stu, n);

    showstu(stu, n);

    sortstu(stu, n);

    char ch[20];
    if (findstu(stu, n, ch)) {
        cout << ch << " is in the student list." << endl;
    } else {
        cout << ch << " is NOT in the student list." << endl;
    }

    return 0;
}
