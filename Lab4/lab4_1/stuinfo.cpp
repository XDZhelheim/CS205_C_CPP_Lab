#include "stuinfo.hpp"

using namespace std;

void inputstu(stuinfo stu[], int n) {
    cout << "Please input information of " << n << " students." << endl;
    for (int i = 0; i < n; i++) {
        cout << "Student " << i << "\'s name:";
        cin.getline(stu[i].name, 20);

        cout << "Student " << i << "\'s scores:";
        cin >> stu[i].score[0] >> stu[i].score[1] >> stu[i].score[2];

        cin.get();

        stu[i].avg = (stu[i].score[0] + stu[i].score[1] + stu[i].score[2]) / 3;
    }
}

void showstu(stuinfo stu[], int n) {
    cout << "The information of " << n << " students you input are:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Student " << i << "\'s name: " << stu[i].name
             << ", scores: " << stu[i].score[0] << " " << stu[i].score[1] << " "
             << stu[i].score[2] << endl;
    }
}

bool cmp(stuinfo stu1, stuinfo stu2) { return stu1.avg > stu2.avg; }

void sortstu(stuinfo stu[], int n) {
    cout << "The decending order of the students:" << endl;
    sort(stu, stu + n, cmp);
    for (int i = 0; i < n; i++) {
        cout << "Student " << i << "\'s name: " << stu[i].name
             << ", scores: " << stu[i].score[0] << " " << stu[i].score[1] << " "
             << stu[i].score[2] << ", average: " << stu[i].avg << endl;
    }
}

bool findstu(stuinfo stu[], int n, char ch[]) {
    cout << "Please input the name you want to find: ";
    cin.getline(ch, 20);
    for (int i = 0; i < n; i++) {
        if (strcmp(ch, stu[i].name) == 0) {
            return true;
        }
    }
    return false;
}