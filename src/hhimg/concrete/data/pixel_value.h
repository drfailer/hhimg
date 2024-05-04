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
    PixelValue(AbstractPixel<T> const &other)
        : red_(other.red()), green_(other.green()), blue_(other.blue()), alpha_(other.alpha()) {}
    PixelValue(std::shared_ptr<AbstractPixel<T>> const &other)
        : red_(other->red()), green_(other->green()), blue_(other->blue()), alpha_(other->alpha()) {}

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

} // namespace hhimg

#endif