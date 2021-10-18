#include "swap.hpp"

void swap_int(int a, int b) {
    int c = a;
    a = b;
    b = c;
}

void swap_int_ptr(int* pa, int* pb) {
    int c = *pa;
    *pa = *pb;
    *pb = c;
}

void swap_int_ref(int& a, int& b) {
    int c = a;
    a = b;
    b = c;
}
