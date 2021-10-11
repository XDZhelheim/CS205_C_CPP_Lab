#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {
    int n = 5;
    char *arr = (char *)malloc(sizeof(char) * n);

    char *p = arr;
    for (int i = 0; i < n - 1; i++) {
        *p = 65 + i;
        p++;
    }
    *p = 0;
    
    printf("%s\n", arr);

    free(arr);
    return 0;
}
