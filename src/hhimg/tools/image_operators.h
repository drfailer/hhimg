#ifndef IMAGE_OPERATORS_HPP
#define IMAGE_OPERATORS_HPP
#include "../abstract/data/abstract_image.h"
#include "pixel_operators.h"
#include <stdexcept>
#include <type_traits>

template <typename InnerType>
concept ValidType =
    std::is_base_of_v<hhimg::AbstractImage<typename InnerType::type>,
                      InnerType> ||
    std::is_base_of_v<hhimg::AbstractPixel<typename InnerType::type>,
                      InnerType>;

// declaration required by operator-=(ImgType<T> &lhs, ImgType<T> const &rhs)
template <ValidType InnerType1, ValidType InnerType2>
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

#define SharedPtrAffectationOp(Op)                                             \
    template <ValidType InnerType1, ValidType InnerType2>                      \
        requires(std::is_same_v<typename InnerType1::type,                     \
                                typename InnerType2::type>)                    \
    std::shared_ptr<InnerType1> const operator Op##=(                          \
        std::shared_ptr<InnerType1> lhs,                                       \
        std::shared_ptr<InnerType2> const rhs) {                               \
        InnerType1 &lhsValue = *lhs;                                           \
        InnerType2 const &rhsValue = *rhs;                                     \
        lhsValue Op## = rhsValue;                                              \
        return lhs;                                                            \
    }
SharedPtrAffectationOp(+) SharedPtrAffectationOp(-) SharedPtrAffectationOp(*)
    SharedPtrAffectationOp(/)
#undef SharedPtrAffectationOp

#define SharedPtrOp(Op)                                                        \
    template <ValidType InnerType1, ValidType InnerType2>                      \
        requires(std::is_same_v<typename InnerType1::type,                     \
                                typename InnerType2::type>)                    \
    std::shared_ptr<hhimg::AbstractPixel<typename InnerType1::type>> const     \
    operator Op(std::shared_ptr<InnerType1> const lhs,                         \
                std::shared_ptr<InnerType2> const rhs) {                       \
        InnerType1 const &lhsValue = *lhs;                                     \
        InnerType2 const &rhsValue = *rhs;                                     \
        return std::make_shared<hhimg::PixelValue<typename InnerType1::type>>( \
            lhsValue Op rhsValue);                                             \
    }
        SharedPtrOp(+) SharedPtrOp(-) SharedPtrOp(*) SharedPtrOp(/)
#undef SharedPtrOp

#endif
