enum Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };

enum Weather { Sunny, Rainy, Snowy, Foggy };

struct Dayinfo
{
    Day day;
    Weather weather;
};

bool canTravel(Dayinfo dayinfo);
