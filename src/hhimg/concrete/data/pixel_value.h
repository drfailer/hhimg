#ifndef PIXEL_VALUE_HPP
#define PIXEL_VALUE_HPP
#include "../../abstract/data/abstract_pixel.h"

namespace hhimg {

/// @brief Default pixel class that can be used for storing temporary values in
/// algorithms.
template <typename T> class PixelValue : public AbstractPixel<T> {
  public:
    PixelValue(T red, T green, T blue, T alpha = 255)
        : red_(red), green_(green), blue_(blue), alpha_(alpha) {}
    PixelValue(std::shared_ptr<AbstractPixel<T>> const &other)
        : PixelValue(other->red(), other->green(), other->blue(),
                     other->alpha()) {}

    T red() const override { return red_; }
    T green() const override { return green_; }
    T blue() const override { return blue_; }
    T alpha() const override { return alpha_; }
    void red(T red) override { red_ = red; }
    void green(T green) override { green_ = green; }
    void blue(T blue) override { blue_ = blue; }
    void alpha(T alpha) override { alpha_ = alpha; }

  private:
    T red_ = 0;
    T green_ = 0;
    T blue_ = 0;
    T alpha_ = 0;
};

namespace pixelOperators {

#define PixelOperator(Op)                                                      \
    template <typename T, typename RhsType>                                    \
    std::shared_ptr<PixelValue<T>> const &operator Op(                         \
        PixelPtr<T> const &pixel, RhsType rhs) {                               \
        auto op = [](T a, T b) { return a Op b; };                             \
        auto result = std::make_shared<PixelValue<T>>(pixel);                  \
        apply(result, rhs, op);                                                \
        return result;                                                         \
    }
PixelOperator(+) PixelOperator(-) PixelOperator(*) PixelOperator(/)
#undef PixelOperator

} // namespace pixelOperators
} // namespace hhimg

#endif
