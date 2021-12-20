#include <iostream>

using std::cout;
using std::endl;

class Car;
class Driver;

class Car {
    friend class Driver;

   private:
    enum { OFF = 0,
           ON = 1 };
    enum { MINVEL,
           MAXVEL = 200 };
    int mode;
    int velocity;

   public:
    Car(int m = ON, int v = 50)
        : mode(m), velocity(v) {}
    bool velup(int v);
    bool veldown(int v);
    bool ison() const;
    int getVel() const;
    void showInfo() const;
};

class Driver {
   public:
    bool velup(Car& car, int v);
    bool veldown(Car& car, int v);
    void setMode(Car& car);
    bool ison(Car& car) const;
};

inline bool Car::velup(int v) {
    if (this->velocity + v > MAXVEL) {
        cout << "Increase velocity by car: The velocity is " << velocity + v << ". It is out of Maxvel." << endl;
        return false;
    }
    this->velocity += v;
    cout << "Increase velocity by car: mode is " << (this->ison() ? "ON" : "OFF") << ", velocity is " << this->velocity << endl;
    return true;
}

inline bool Car::veldown(int v) {
    if (this->velocity - v < MINVEL) {
        cout << "Decrease velocity by car: The velocity is " << velocity - v << ". It is out of Minvel." << endl;
        return false;
    }
    this->velocity -= v;
    cout << "Decrease velocity by car: mode is " << (this->ison() ? "ON" : "OFF") << ", velocity is " << this->velocity << endl;
    return true;
}

inline bool Car::ison() const {
    return this->mode == ON;
}

inline int Car::getVel() const {
    return this->velocity;
}

inline void Car::showInfo() const {
    cout << "The infomation of car: mode is " << (this->ison() ? "ON" : "OFF") << ", velocity is " << this->velocity << endl;
}

inline bool Driver::ison(Car& car) const {
    return car.ison();
}

inline bool Driver::velup(Car& car, int v) {
    if (car.velocity + v > Car::MAXVEL) {
        cout << "Increase velocity by driver: The velocity is " << car.velocity + v << ". It is out of Maxvel." << endl;
        return false;
    }
    car.velocity += v;
    cout << "Increase velocity by car: mode is " << (this->ison(car) ? "ON" : "OFF") << ", velocity is " << car.velocity << endl;
    return true;
}

inline bool Driver::veldown(Car& car, int v) {
    if (car.velocity - v < Car::MINVEL) {
        cout << "Decrease velocity by driver: The velocity is " << car.velocity - v << ". It is out of Minvel." << endl;
        return false;
    }
    car.velocity -= v;
    cout << "Decrease velocity by car: mode is " << (this->ison(car) ? "ON" : "OFF") << ", velocity is " << car.velocity << endl;
    return true;
}

inline void Driver::setMode(Car& car) {
    cout << "Set the mode of car by driver:" << endl;
    car.mode = !car.mode;
    cout << "The mode of the car is: " << (this->ison(car) ? "ON" : "OFF") << endl;
}

int main(int argc, char const* argv[]) {
    Car car;
    Driver driver;

    car.showInfo();
    car.velup(120);
    driver.setMode(car);
    car.showInfo();
    driver.veldown(car, 100);
    driver.velup(car, 150);
    return 0;
}
