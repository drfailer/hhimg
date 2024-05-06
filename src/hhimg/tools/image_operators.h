#ifndef IMAGE_OPERATORS_HPP
#define IMAGE_OPERATORS_HPP
#include "../abstract/data/abstract_image.h"
#include "concepts.h"
#include "pixel_operators.h"
#include <stdexcept>
#include <type_traits>

// declaration required by operator-=(ImgType<T> &lhs, ImgType<T> const &rhs)
template <hhimg::concepts::ValidType InnerType1,
          hhimg::concepts::ValidType InnerType2>
    requires(
        std::is_same_v<typename InnerType1::type, typename InnerType2::type>)
std::shared_ptr<InnerType1> const
operator-=(std::shared_ptr<InnerType1> lhs,
           std::shared_ptr<InnerType2> const rhs);

template <typename T> using ImgType = hhimg::AbstractImage<T>;

// operator-=
template <typename T>
ImgType<T> const &operator-=(ImgType<T> &lhs, ImgType<T> const &rhs) {
    if (lhs.width() != rhs.width() || lhs.height() != rhs.height()) {
        throw std::invalid_argument(
            "error: can't substract images with different shapes");
    }
    for (size_t y = 0; y < lhs.height(); ++y) {
        for (size_t x = 0; x < lhs.width(); ++x) {
            lhs.at(x, y) -= rhs.at(x, y);
        }
    }
    return lhs;
}

// operator-
template <typename T>
ImgType<T> const &operator-(ImgType<T> const &lhs, ImgType<T> const &rhs) {
    if (lhs->width() != rhs->width() || lhs->height() != rhs->height()) {
        throw std::invalid_argument(
            "error: can't substract images with different shapes");
    }
    auto result = lhs->copy();
    return result -= rhs;
}

#endif
