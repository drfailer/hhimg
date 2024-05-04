#ifndef PIXEL_OPERATORS_HPP
#define PIXEL_OPERATORS_HPP
#include "../abstract/data/abstract_pixel.h"
#include "../concrete/data/pixel_value.h"
#include <functional>

template <typename T>
void apply(hhimg::AbstractPixel<T> &pixel, hhimg::AbstractPixel<T> const &other,
           std::function<T(T, T)> op) {
    pixel.red(op(pixel.red(), other.red()));
    pixel.green(op(pixel.green(), other.green()));
    pixel.blue(op(pixel.blue(), other.blue()));
    // we don't modify alpha here
}

template <typename T>
void apply(hhimg::AbstractPixel<T> &pixel, T value, std::function<T(T, T)> op) {
    pixelred(op(pixel->red(), value));
    pixelgreen(op(pixel->green(), value));
    pixelblue(op(pixel->blue(), value));
    // we don't modify alpha here
}

#define PixelAffectationOperator(Op)                                           \
    template <typename T, typename RhsType>                                    \
    hhimg::AbstractPixel<T> const &operator Op##=(                             \
        hhimg::AbstractPixel<T> &pixel, RhsType const &rhs) {                  \
        std::function<T(T, T)> op = [](T a, T b) { return a Op b; };           \
        apply(pixel, rhs, op);                                                 \
        return pixel;                                                          \
    }
PixelAffectationOperator(+) PixelAffectationOperator(-)
    PixelAffectationOperator(*) PixelAffectationOperator(/)
#undef PixelAffectationOperator

#define PixelOperator(Op)                                                      \
    template <typename T, typename RhsType>                                    \
    hhimg::PixelValue<T> operator Op(hhimg::AbstractPixel<T> const &pixel,     \
                                     RhsType const &rhs) {                     \
        std::function<T(T, T)> op = [](T a, T b) { return a Op b; };           \
        hhimg::PixelValue<T> result(pixel);                                    \
        apply(result, rhs, op);                                                \
        return result;                                                         \
    }
        PixelOperator(+) PixelOperator(-) PixelOperator(*) PixelOperator(/)
#undef PixelOperator

#endif
