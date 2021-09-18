#include "fac.hpp"

long long fac(int n) { return n == 1 ? 1 : n * fac(n - 1); }