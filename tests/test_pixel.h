#ifndef TEST_PIXEL_HPP
#define TEST_PIXEL_HPP
#include <hhimg/hhimg.h>

template <typename T> class TestPixel : public hhimg::AbstractPixel<T> {
  public:
    TestPixel(T &red, T &green, T &blue)
        : red_(red), green_(green), blue_(blue) {}
    TestPixel(std::shared_ptr<hhimg::AbstractPixel<T>> const &other)
        : TestPixel(other->red(), other->green(), other->blue(),
                    other->alpha()) {}
    TestPixel(hhimg::AbstractPixel<T> const &other)
        : TestPixel(other.red(), other.green(), other.blue(),
                    other.alpha()) {}

    T red() const override { return red_; }
    T green() const override { return green_; }
    T blue() const override { return blue_; }
    T alpha() const override { return alpha_; }
    void red(T red) override { red_ = red; }
    void green(T green) override { green_ = green; }
    void blue(T blue) override { blue_ = blue; }
    void alpha(T alpha) override { alpha_ = alpha; }

  private:
    T &red_ = 0;
    T &green_ = 0;
    T &blue_ = 0;
    T alpha_ = 0;
};

#endif
