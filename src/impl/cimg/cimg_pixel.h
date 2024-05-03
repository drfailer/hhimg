#ifndef CIMG_PIXEL_HPP
#define CIMG_PIXEL_HPP
#include <hhimg/hhimg.h>

template <typename T>
class CImgPixel: public hhimg::AbstractPixel<T> {
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
    void red(T red) override { red_ = red; }
    void green(T green) override { green_ = green; }
    void blue(T blue) override { blue_ = blue; }
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
