#ifndef CIMG_PIXEL_HPP
#define CIMG_PIXEL_HPP
#include "../../api/data/abastract_pixel.h"

template <typename T>
class CImgPixel: public AbstractPixel<T> {
  public:
    CImgPixel(T &red, T &green, T &blue):
        red_(red), green_(green), blue_(blue) {}
    CImgPixel(CImgPixel<T> const &other):
        CImgPixel(other.red_, other.green_, other.blue_, other.alpha_) {}
    CImgPixel(CImgPixel<T> &&other):
        CImgPixel(other.red_, other.green_, other.blue_, other.alpha_) {}

    T red() const override { return red_; }
    T green() const override { return green_; }
    T blue() const override { return blue_; }
    T alpha() const override { return 255; }
    void red(T red) override { red_ = this->validate(red); }
    void green(T green) override { green_ = this->validate(green); }
    void blue(T blue) override { blue_ = this->validate(blue); }
    void alpha(T) override {}

    void set(T red, T green, T blue, T) override {
        this->red_ = red;
        this->green_ = green;
        this->blue_ = blue;
    }

  private:
    T &red_ = nullptr;
    T &green_ = nullptr;
    T &blue_ = nullptr;
};

#endif
