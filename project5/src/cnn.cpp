#include "cnn.hpp"

inline void CNN::add_layer(Layer* l) {
    this->layers.push_back(l);
}

inline Mat CNN::predict(Mat& image) {
    Mat res = image;
    for (auto l : this->layers) {
        res = l->forward(res);
    }
    return res;
}
