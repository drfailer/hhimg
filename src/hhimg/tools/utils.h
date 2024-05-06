#ifndef UTILS_HPP
#define UTILS_HPP
#include "../abstract/data/abstract_image.h"
#include "../abstract/data/abstract_pixel.h"

namespace hhimg::utils {

template <typename T> T validate(T value) {
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return value;
}

} // namespace hhimg::utils

#endif
