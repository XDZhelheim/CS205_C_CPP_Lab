#include "swap.hpp"

int main(int argc, char const *argv[])
{
    int a=1, b=2;
    swap_int(a, b);
    printf("a = %d, b = %d\n", a, b);

    a=1, b=2;
    swap_int_ptr(&a, &b);
    printf("a = %d, b = %d\n", a, b);

    a=1, b=2;
    swap_int_ref(a, b);
    printf("a = %d, b = %d\n", a, b);
    return 0;
}
