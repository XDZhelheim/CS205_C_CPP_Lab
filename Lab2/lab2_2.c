#include <stdio.h>
#include <stdbool.h>

int main(int argc, char const *argv[])
{
    printf("\nPrint size of the fundamental types:\n");
    printf("-----------------------------------------\n");
    printf("The sizeof(char) is:  %lld bytes.\n", sizeof(char));
    printf("The sizeof(short) is:  %lld bytes.\n", sizeof(short));
    printf("The sizeof(int) is:  %lld bytes.\n", sizeof(int));
    printf("The sizeof(long) is:  %lld bytes.\n", sizeof(long));
    printf("The sizeof(long long) is:  %lld bytes.\n", sizeof(long long));
    printf("The sizeof(float) is:  %lld bytes.\n", sizeof(float));
    printf("The sizeof(double) is:  %lld bytes.\n", sizeof(double));
    printf("The sizeof(long double) is:  %lld bytes.\n", sizeof(long double));
    printf("The sizeof(bool) is:  %lld byte.\n", sizeof(bool));
    return 0;
}
