#include <iostream>
#include <algorithm>
#include <cstring>

struct stuinfo {
    char name[20];
    double score[3];
    double avg;
};

// asks the user to enter each of the preceding items of information to set the corresponding members of the structure
void inputstu(stuinfo stu[], int n);

// displays the contents of the structure, one student one line
void showstu(stuinfo stu[], int n);

// sorts in descending order of  average of three scores
void sortstu(stuinfo stu[], int n);

// finds if given characters is the student’s name
bool findstu(stuinfo stu[], int n, char ch[]);