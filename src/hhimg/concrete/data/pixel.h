#ifndef PIXEL_HPP
#define PIXEL_HPP
#include "../../tools/utils.h"

namespace hhimg {

template <typename T> struct Pixel {
    T red = 0;
    T green = 0;
    T blue = 0;
};

template <typename Out, typename In>
Pixel<Out> validate(Pixel<In> const &pixel) {
    return {(Out)utils::validate(pixel.red), (Out)utils::validate(pixel.green),
            (Out)utils::validate(pixel.blue)};
}

#define PixelOpPixel(Op)                                                       \
    template <typename T>                                                      \
    Pixel<T> operator Op(Pixel<T> const &lhs, Pixel<T> const &rhs) {           \
        return {(T)utils::validate(lhs.red Op rhs.red),                        \
                (T)utils::validate(lhs.green Op rhs.green),                    \
                (T)utils::validate(lhs.blue Op rhs.blue)};                     \
    }
PixelOpPixel(+) PixelOpPixel(-) PixelOpPixel(/) PixelOpPixel(*)
#undef PixelOp

#define PixelOpValue(Op)                                                       \
    template <typename T, typename RhsT>                                       \
    Pixel<RhsT> operator Op(Pixel<T> const &lhs, RhsT rhs) {                   \
        return {lhs.red Op rhs, lhs.green Op rhs, lhs.blue Op rhs};            \
    }
    PixelOpValue(+) PixelOpValue(-) PixelOpValue(/) PixelOpValue(*)
#undef PixelOpValue

#define PixelOpEqPixel(Op)                                                     \
    template <typename T>                                                      \
    Pixel<T> const &operator Op##=(Pixel<T> &lhs, Pixel<T> const &rhs) {       \
        lhs.red Op## = rhs.red;                                                \
        lhs.green Op## = rhs.green;                                            \
        lhs.blue Op## = rhs.blue;                                              \
        return lhs;                                                            \
    }
        PixelOpEqPixel(+) PixelOpEqPixel(-) PixelOpEqPixel(/) PixelOpEqPixel(*)
#undef PixelOpEq

#define PixelOpEqValue(Op)                                                     \
    template <typename T, typename RhsT>                                       \
    const Pixel<T> &operator Op##=(Pixel<T> &lhs, RhsT const rhs) {            \
        lhs.red Op## = rhs;                                                    \
        lhs.green Op## = rhs;                                                  \
        lhs.blue Op## = rhs;                                                   \
        return lhs;                                                            \
    }
            PixelOpEqValue(+) PixelOpEqValue(-) PixelOpEqValue(/)
                PixelOpEqValue(*)
#undef PixelOpEqValue

} // namespace hhimg

#endif
