#include <iostream>

using namespace std;

void printFloat(float num)
{
    int inum = *(int *)&num;
    for (int i = 0; i < 32; ++i)
    {
        cout << ((inum & 0x80000000) ? 1 : 0);
        if (i == 0 || i == 8)
            cout << " ";
        inum <<= 1;
    }
    cout << endl;
}

int main()
{
    float f1 = 1.0;
    cout << "f1 = " << f1 << endl;
    cout << "The binary presentaion of f1"
         << " is:" << endl;
    printFloat(f1);
    cout << endl;

    float a = 0.1;
    float f2 = a + a + a + a + a + a + a + a + a + a;
    cout << "f2 = " << f2 << endl;
    cout << "The binary presentaion of f2"
         << " is:" << endl;
    printFloat(f2);
    cout << endl;

    return 0;
}
