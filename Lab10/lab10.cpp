#include "complex.hpp"

int main(int argc, char const *argv[])
{
    complex a1(1, 2);
    complex a;
    a=a1;
    complex b;
    cin>>b;

    complex c=a+b;

    cout<<a+b<<endl;
    cout<<a-b<<endl;
    cout<<a*b<<endl;
    cout<<(a==b)<<endl;
    cout<<(a!=b)<<endl;

    return 0;
}
