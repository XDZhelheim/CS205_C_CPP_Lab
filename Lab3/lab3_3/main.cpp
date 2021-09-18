#include <iostream>

#include "operation.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    printf("Please select an operation:\n+\n-\n*\n/\n%\n");
    char op=0;
    scanf("%c", &op);
    while (op!='+' && op!='-' && op!='*' && op!='/' && op!='%') {
        printf("Invalid operator, please re-input:\n");
        scanf("%c", &op);
    }

    printf("Please input two integers:\n");
    ll a, b;
    scanf("%lld%lld", &a, &b);

    while ((op=='/' || op=='%') && b==0) {
        printf("The dividend cannot be 0, please re-input:\n");
        scanf("%lld", &b);
    }

    ll res;
    switch (op) {
        case '+':
            res=add(a, b);
            break;

        case '-':
            res=substract(a, b);
            break;

        case '*':
            res=multiply(a, b);
            break;

        case '/':
            res=divide(a, b);
            break;

        case '%':
            res=modulo(a, b);
            break;
        
        default:
            break;
    }

    printf("%lld %c %lld = %lld\n", a, op, b, res);

    return 0;
}
