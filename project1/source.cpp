#include <iostream>
#include <regex>
#include <string>

#define LF 10     // ASCII code for \n
#define SP 32     // ASCII code for space
#define NUL 0     // ASCII code for null
#define MINUS 45  // ASCII code for '-'

#define DEBUG 0

using namespace std;

/**
 * Regular expression for input check
 *
 * invalid: +++12345 or -+--+6789
 * valid: +012345, -000234
 */
regex number_regexp("(\\+?|-)\\d+");

bool check_valid(string input_str) {
    return regex_match(input_str, number_regexp);
}

void read_input(string &multiplicand, string &multiplier) {
    cin >> multiplicand >> multiplier;

    char nextchar;
    while ((nextchar = getchar()) != LF) {
        if (nextchar != SP) {
            fflush(stdin);
            cout << "Wrong number of input, please input two integers:" << endl;
            cin >> multiplicand >> multiplier;
        }
    }
}

int get_sign(string &number) {
    int sign;
    if (number[0] == '-') {
        sign = -1;
        number = &number[1];
    } else {
        sign = 1;
        if (number[0] == '+')
            number = &number[1];
    }
    return sign;
}

string multiply(string &multiplicand, string &multiplier) {
    int sign1 = get_sign(multiplicand);
    int sign2 = get_sign(multiplier);
    char sign = sign1 * sign2 > 0 ? NUL : MINUS;

    int sum_len = multiplicand.length() + multiplier.length();
    int temp[sum_len];
    memset(temp, 0, sizeof(temp));

    for (int i = multiplier.length() - 1; i >= 0; i--)
        for (int j = multiplicand.length() - 1; j >= 0; j--) {
            if (DEBUG)
                cout << "i=" << i << ", j=" << j << ": " << multiplier[i] - '0'
                     << "*" << multiplicand[j] - '0' << endl;
            temp[sum_len - 2 - (i + j)] += (multiplicand[j] - '0') * (multiplier[i] - '0');
        }

    for (int i = 0; i < sum_len; i++) {
        temp[i + 1] += temp[i] / 10;
        temp[i] %= 10;
    }

    if (DEBUG) {
        cout << "array = ";
        for (int i = 0; i < sum_len; i++) {
            cout << temp[i];
        }
        cout << endl;
    }

    string result;
    result.push_back(sign);
    bool flag = 0;
    for (int i = sum_len - 1; i >= 0; i--) {
        if (temp[i] != 0) 
            flag = 1;
        if (flag) 
            result.push_back(temp[i] + '0');
    }

    return result;
}

int main(int argc, char const *argv[]) {
    string multiplicand, multiplier;

    if (argc == 3) {
        multiplicand = argv[1];
        multiplier = argv[2];
    } else {
        if (argc != 1)
            cout << "Wrong number of arguments, please input two integers:" << endl;
        else
            cout << "Please input two integers:" << endl;

        read_input(multiplicand, multiplier);
    }

    while (!(check_valid(multiplicand) && check_valid(multiplier))) {
        cout << "Invalid input, please input two integers:" << endl;
        read_input(multiplicand, multiplier);
    }

    cout << multiplicand << " * " << multiplier << " = "
         << multiply(multiplicand, multiplier) << endl;

    return 0;
}
