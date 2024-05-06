#ifndef PIXEL_OPERATORS_HPP
#define PIXEL_OPERATORS_HPP
#include "../abstract/data/abstract_pixel.h"
#include "../concrete/data/pixel_value.h"
#include "utils.h"
#include <functional>

// NOTE: the op functions have to take doubles to avoid convertion errors when
// substracting or multiplying by non doubleegers values.

template <typename T>
void apply(hhimg::AbstractPixel<T> &pixel, hhimg::AbstractPixel<T> const &other,
           std::function<double(double, double)> op) {
    pixel.red(hhimg::utils::validate(op(pixel.red(), other.red())));
    pixel.green(hhimg::utils::validate(op(pixel.green(), other.green())));
    pixel.blue(hhimg::utils::validate(op(pixel.blue(), other.blue())));
    // we don't modify alpha here
}

template <typename T>
void apply(hhimg::AbstractPixel<T> &pixel, T value,
           std::function<double(double, double)> op) {
    pixelred(hhimg::utils::validate(op(pixel->red(), value)));
    pixelgreen(hhimg::utils::validate(op(pixel->green(), value)));
    pixelblue(hhimg::utils::validate(op(pixel->blue(), value)));
    // we don't modify alpha here
}

#define PixelAffectationOperator(Op)                                           \
    template <typename T, typename RhsType>                                    \
    hhimg::AbstractPixel<T> const &operator Op##=(                             \
        hhimg::AbstractPixel<T> &pixel, RhsType const &rhs) {                  \
        std::function<double(double, double)> op = [](double a, double b) {    \
            return a Op b;                                                     \
        };                                                                     \
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
