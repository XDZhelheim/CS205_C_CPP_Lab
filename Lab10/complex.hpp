#include <iostream>

using namespace std;

class complex {
   private:
    double real;
    double imag;

   public:
    complex() {
        this->real = 0;
        this->imag = 0;
    }

    complex(double real, double imag) {
        this->real = real;
        this->imag = imag;
    }

    complex operator+(complex& other) {
        return complex(this->real + other.real, this->imag + other.imag);
    }

    complex operator-(complex& other) {
        return complex(this->real - other.real, this->imag - other.imag);
    }

    complex operator*(complex& other) {
        return complex(this->real * other.real - this->imag * other.imag, this->real * other.imag + this->imag * other.real);
    }

    void operator=(complex& other) {
        this->real = other.real;
        this->imag = other.imag;
    }

    bool operator==(complex& other) {
        return this->real == other.real && this->imag == other.imag;
    }

    bool operator!=(complex& other) {
        return this->real != other.real && this->imag != other.imag;
    }

    friend ostream& operator<<(ostream& os, const complex& c) {
        if (c.imag > 0) {
            os << c.real << "+" << c.imag << "i";
        } else {
            os << c.real << c.imag << "i";
        }

        return os;
    }

    friend istream& operator>>(istream& is, complex& c) {
        is >> c.real;
        is >> c.imag;
        return is;
    }
};
