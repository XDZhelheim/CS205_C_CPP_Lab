#include <iostream>

#include "Dayinfo.hpp"

using namespace std;

bool canTravel(Dayinfo dayinfo) {
    bool flag1 = false, flag2 = false;
    if (dayinfo.day == Saturday || dayinfo.day == Sunday) {
        flag1 = true;
    }
    if (dayinfo.weather == Sunny) {
        flag2 = true;
    }
    return flag1 && flag2;
}

int main(int argc, char const *argv[])
{
    Dayinfo d1={Saturday, Sunny};
    Dayinfo d2={Monday, Sunny};
    Dayinfo d3={Sunday, Snowy};

    cout<<canTravel(d1)<<endl;
    cout<<canTravel(d2)<<endl;
    cout<<canTravel(d3)<<endl;
    return 0;
}
