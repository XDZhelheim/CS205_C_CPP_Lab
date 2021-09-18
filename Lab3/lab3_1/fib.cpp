#include "fib.hpp"

int fib(int n) { return (n == 1 || n == 2) ? 1 : fib(n - 1) + fib(n - 2); }