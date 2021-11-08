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

    static complex add(const complex& a, const complex& b) {
        return complex(a.real + b.real, a.imag + b.imag);
    }

    static complex sub(const complex& a, const complex& b) {
        return complex(a.real - b.real, a.imag - b.imag);
    }

    void display() const {
        if (this->imag > 0) {
            cout << this->real << "+" << this->imag << "i" << endl;
        } else {
            cout << this->real << this->imag << "i" << endl;
        }
    }
};