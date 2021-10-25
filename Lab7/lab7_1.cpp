#include <iostream>

using namespace std;

void displaySquare(int side = 4, char filledCharacter = '*');

int main(int argc, char const* argv[]) {
    displaySquare();
    putchar('\n');
    displaySquare(5, '#');
    return 0;
}

void displaySquare(int side, char filledCharacter) {
    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            putchar(filledCharacter);
        }
        putchar('\n');
    }
}
