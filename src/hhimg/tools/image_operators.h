#ifndef IMAGE_OPERATORS_HPP
#define IMAGE_OPERATORS_HPP
#include <memory>
#include <stdexcept>
#include "pixel_operators.h"

// operator-=
template <typename Img>
std::shared_ptr<Img> const &operator-=(std::shared_ptr<Img> &lhs,
                                       std::shared_ptr<Img> const &rhs) {
    if (lhs->width() != rhs->width() || lhs->height() != rhs->height()) {
        throw std::invalid_argument(
            "error: can't substract images with different shapes");
    }
    for (size_t y = 0; y < lhs->height(); ++y) {
        for (size_t x = 0; x < lhs->width(); ++x) {
            lhs->at(x, y) -= rhs->at(x, y);
        }
    }
    return lhs;
}

// operator-
template <typename Img>
std::shared_ptr<Img> const &operator-(std::shared_ptr<Img> const &lhs,
                                      std::shared_ptr<Img> const &rhs) {
    if (lhs->width() != rhs->width() || lhs->height() != rhs->height()) {
        throw std::invalid_argument(
            "error: can't substract images with different shapes");
    }
    auto result = lhs->copy();
    return result -= rhs;
}

#endif
