#include <iostream>
#include <cassert>

using namespace std;

float calculateAvg(float* marks, int length) {
    float sum = 0;
    for (int i = 0; i < length; i++) {
        assert(marks[i] >= 0 && marks[i] <= 100);
        sum += marks[i];
    }
    return sum / length;
}

int main(int argc, char const* argv[]) {
    float marks[4];
    string control = "y";
    while (control == "y") {
        cout << "Please enter marks for 4 courses: ";
        for (int i = 0; i < 4; i++) {
            cin >> marks[i];
        }
        float avg = calculateAvg(marks, 4);

        cout << "The average of the 4 courses is " << avg << endl;

        cout << "Would you want to enter another 4 marks for 4 courses? (y/n) ";
        cin >> control;
        if (control == "n") {
            break;
        }
    }
    cout << "Bye, see you next time." << endl;
    return 0;
}
