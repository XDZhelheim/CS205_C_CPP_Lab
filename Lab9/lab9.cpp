#include "complex.hpp"

int main(int argc, char const *argv[])
{
    complex a;
    complex b(1, 2);
    complex c=complex::add(a, b);
    complex d=complex::sub(a, b);

    c.display();
    d.display();
    return 0;
}
