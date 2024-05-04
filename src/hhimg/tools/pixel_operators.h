#ifndef PIXEL_OPERATORS_HPP
#define PIXEL_OPERATORS_HPP
#include "../abstract/data/abstract_pixel.h"
#include "../concrete/data/pixel_value.h"
#include <functional>

template <typename T> using PixelPtr = std::shared_ptr<hhimg::AbstractPixel<T>>;

template <typename T>
void apply(PixelPtr<T> pixel, PixelPtr<T> other, std::function<T(T, T)> op) {
    pixel->red(op(pixel->red(), other->red()));
    pixel->green(op(pixel->green(), other->green()));
    pixel->blue(op(pixel->blue(), other->blue()));
    // we don't modify alpha here
}

template <typename T>
void apply(PixelPtr<T> pixel, T value, std::function<T(T, T)> op) {
    pixel->red(op(pixel->red(), value));
    pixel->green(op(pixel->green(), value));
    pixel->blue(op(pixel->blue(), value));
    // we don't modify alpha here
}

#define PixelAffectationOperator(Op)                                           \
    template <typename T, typename RhsType>                                    \
    PixelPtr<T> operator Op##=(PixelPtr<T> pixel, RhsType rhs) {               \
        std::function<T(T, T)> op = [](T a, T b) { return a Op b; };           \
        apply(pixel, rhs, op);                                                 \
        return pixel;                                                          \
    }
PixelAffectationOperator(+) PixelAffectationOperator(-)
    PixelAffectationOperator(*) PixelAffectationOperator(/)
#undef PixelAffectationOperator

#define PixelOperator(Op)                                                      \
    template <typename T, typename RhsType>                                    \
    std::shared_ptr<hhimg::PixelValue<T>> const &operator Op(                  \
        PixelPtr<T> const &pixel, RhsType rhs) {                               \
        auto op = [](T a, T b) { return a Op b; };                             \
        auto result = std::make_shared<hhimg::PixelValue<T>>(pixel);           \
        apply(result, rhs, op);                                                \
        return result;                                                         \
    }
        PixelOperator(+) PixelOperator(-) PixelOperator(*) PixelOperator(/)
#undef PixelOperator

#endif
