#pragma once

#include <iostream>

#define PI 3.14

using namespace std;

class CStereoShape {
   private:
    static int numberOfObject;

   public:
    CStereoShape() {
        this->numberOfObject++;
    }

    CStereoShape(const CStereoShape& other) {
        this->numberOfObject++;
    }

    static int getNumOfObject() {
        return numberOfObject;
    }

    virtual double getArea() {
        return 0.0;
    }

    virtual double getVolume() {
        return 0.0;
    }

    virtual void show() {
        cout << "CStereoShape::show()" << endl;
    }
};

class CCube : public CStereoShape {
   private:
    double length, width, height;

   public:
    CCube()
        : CStereoShape() {}
    CCube(double length, double width, double height)
        : CStereoShape() {
        this->length = length;
        this->width = width;
        this->height = height;
    }
    CCube(const CCube& other)
        : CStereoShape(other) {
        this->length = other.length;
        this->width = other.width;
        this->height = other.height;
    }

    double getArea() override {
        return 2 * (this->length * this->width + this->length * this->height + this->width * this->height);
    }

    double getVolume() override {
        return this->length * this->width * this->height;
    }

    void show() override {
        cout << "Cube length: " << this->length << "\twidth: " << this->width << "\theight: " << this->height << endl;
        cout << "Cube area: " << this->getArea() << "\tvolume: " << this->getVolume() << endl;
    }
};

class CSphere : public CStereoShape {
   private:
    double radius;

   public:
    CSphere()
        : CStereoShape() {}
    CSphere(double radius)
        : CStereoShape() {
        this->radius = radius;
    }
    CSphere(const CSphere& other)
        : CStereoShape(other) {
        this->radius = other.radius;
    }

    double getArea() override {
        return 4 * PI * this->radius * this->radius;
    }

    double getVolume() override {
        return (double)4 / (double)3 * PI * this->radius * this->radius * this->radius;
    }

    void show() override {
        cout.flags(ios::fixed);
        cout.precision(2);
        cout << "Sphere radius: " << this->radius << "\tarea: " << this->getArea() << "\tvolume: " << this->getVolume() << endl;
    }
};
