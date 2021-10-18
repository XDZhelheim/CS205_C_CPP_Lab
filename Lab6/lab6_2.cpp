#include <iostream>

using namespace std;

char* match(char* s, char ch);

int main(int argc, char const* argv[]) {
    printf("Please input a string:\n");
    char str[1024];
    cin.getline(str, 1024);

    printf("Please input a character:\n");
    char c = getchar();

    char* p = match(str, c);

    p ? printf("%s\n", p) : printf("Not found.\n");

    return 0;
}

char* match(char* s, char ch) {
    char* pchar = s;
    while (*pchar != 0) {
        if (*pchar == ch) {
            return pchar;
        }
        pchar++;
    }
    return NULL;
}
