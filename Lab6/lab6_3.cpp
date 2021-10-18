#include <iostream>

using namespace std;

struct point {
    float x;
    float y;
};

point create_point(float x, float y) {
    point p = {
        .x = x,
        .y = y};

    return p;
}

point mid_point(point* p1, point* p2) {
    point p = {
        .x = (p1->x + p2->x) / 2,
        .y = (p1->y + p2->y) / 2};

    return p;
}

int main(int argc, char const* argv[]) {
    point p1 = create_point(3, 5);
    point p2 = create_point(1, 1);

    point mid = mid_point(&p1, &p2);
    cout << "(" << mid.x << ", " << mid.y << ")" << endl;

    return 0;
}
