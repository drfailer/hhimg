#ifndef UTILS_HPP
#define UTILS_HPP
#include "../abstract/data/abstract_image.h"
#include "../abstract/data/abstract_pixel.h"
#include <ostream>
#include <memory>

namespace hhimg::utils {

template <typename T> T validate(T value) {
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return value;
}

} // namespace hhimg::utils

template <typename T>
std::ostream &operator<<(std::ostream &os, const hhimg::AbstractPixel<T> &pixel) {
    os << "pix: { " << (int) pixel.red() << ", " << (int) pixel.green() << ", "
       << (int) pixel.blue() << " }";
    return os;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, std::shared_ptr<hhimg::AbstractPixel<T>> pixel) {
    os << *pixel;
    return os;
}

#endif
