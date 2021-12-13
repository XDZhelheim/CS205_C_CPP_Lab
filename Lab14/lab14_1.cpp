#include <iostream>

using namespace std;

class OutOfRangeException {
   public:
    int error_pos;
    float error_num;
    
    OutOfRangeException(int error_pos, float error_num) {
        this->error_pos = error_pos;
        this->error_num = error_num;
    }

    void print() {
        cout << "The parameter " << this->error_pos << " is " << this->error_num << " which is out of range (0-100)" << endl;
    }
};

float calculateAvg(float* marks, int length) {
    float sum = 0;
    for (int i = 0; i < length; i++) {
        if (marks[i] < 0 || marks[i] > 100) {
            throw OutOfRangeException(i + 1, marks[i]);
        }
        sum += marks[i];
    }
    return sum / length;
}

int main(int argc, char const* argv[]) {
    float marks[4];
    bool flag = true;
    string control = "y";
    while (control == "y") {
        cout << "Please enter marks for 4 courses: ";
        for (int i = 0; i < 4; i++) {
            cin >> marks[i];
        }
        float avg;
        try {
            avg = calculateAvg(marks, 4);
        } catch (OutOfRangeException& e) {
            e.print();
            flag = false;
        }

        if (flag) {
            cout << "The average of the 4 courses is " << avg << endl;
        }

        cout << "Would you want to enter another 4 marks for 4 courses? (y/n) ";
        cin >> control;
        if (control == "n") {
            break;
        }
    }
    cout << "Bye, see you next time." << endl;
    return 0;
}
