#ifndef RGB_VALUE_HPP
#define RGB_VALUE_HPP
#include <cstddef>
#include <random>
#include <hhimg/hhimg.h>

template <typename T> struct RGBValue {
    T red;
    T green;
    T blue;

    void set(T r, T g, T b) {
        red = r;
        green = g;
        blue = b;
    }

    void set(T value) {
        red = value;
        green = value;
        blue = value;
    }
};

template <typename T>
bool operator==(RGBValue<T> const &lhs, RGBValue<T> const &rhs) {
    return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue;
}

template <typename T>
bool operator==(hhimg::Pixel<T> const &lhs, RGBValue<T> const &rhs) {
    return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue;
}

template <typename T>
RGBValue<T> *randomRGBValues(size_t width, size_t height) {
    RGBValue<T> *mem = new RGBValue<T>[width * height];
    std::mt19937 gen;

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            mem[y * width + x].set(gen() % 255, gen() % 255, gen() % 255);
        }
    }
    return mem;
}

template <typename T> bool isGray(RGBValue<T> const &v) {
    return v.red == v.green && v.green == v.blue;
}

template <typename T>
bool isGrayScaled(RGBValue<T> *image, size_t width, size_t height) {
    for (size_t i = 0; i < height * width; ++i) {
        if (!isGray(image[i])) {
            return false;
        }
    }
    return true;
}

#endif
