#ifndef ABASTRACT_PIXEL_HPP
#define ABASTRACT_PIXEL_HPP
#include <memory>

namespace hhimg {

template <typename T> class AbstractPixel {
  public:
    AbstractPixel() = default;
    virtual ~AbstractPixel() {}

    virtual T red() const = 0;
    virtual T green() const = 0;
    virtual T blue() const = 0;
    virtual T alpha() const = 0;
    virtual void red(T red) = 0;
    virtual void green(T green) = 0;
    virtual void blue(T blue) = 0;
    virtual void alpha(T alpha) = 0;

    // set
    void set(T red, T green, T blue, T alpha = 0) {
        this->red(red);
        this->green(green);
        this->blue(blue);
        this->alpha(alpha);
    }
    void set(T value) { set(value, value, value); }
    void set(std::shared_ptr<AbstractPixel<T>> const other) {
        set(other->red(), other->green(), other->blue(), other->alpha());
    }
    void set(AbstractPixel<T> const &other) {
        set(other.red(), other.green(), other.blue(), other.alpha());
    }

    // function for improving readability when working with gray images
    // (warn: we do not test if the pixel is actually gray)
    T get() const { return red(); }

    // type
    using type = T;
};

} // namespace hhimg

#endif
