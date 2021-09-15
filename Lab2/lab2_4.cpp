#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    unsigned int a=-1;
    int MAX=a>>1; // 算术右移
    int MIN=MAX+1;
    printf("[%d, %d]\n", MIN, MAX);
    return 0;
}
