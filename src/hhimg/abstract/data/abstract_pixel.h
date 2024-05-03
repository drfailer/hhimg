#ifndef ABASTRACT_PIXEL_HPP
#define ABASTRACT_PIXEL_HPP
#include <functional>
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

    virtual void set(T red, T green, T blue, T alpha = 0) = 0;
    void set(T value) { set(value, value, value); }
    void set(std::shared_ptr<AbstractPixel<T>> const other) {
        set(other->red(), other->green(), other->blue(), other->alpha());
    }

    // function for improving readability when working with gray images
    // (warn: we do not test if the pixel is actually gray)
    T get() const { return red(); }
};

/******************************************************************************/
/*                                 operators                                  */
/******************************************************************************/

namespace pixelOperators {

template <typename T> using PixelPtr = std::shared_ptr<AbstractPixel<T>>;

template <typename T>
void apply(PixelPtr<T> &pixel, PixelPtr<T> const &other,
           std::function<T(T, T)> op) {
    pixel->red(op(pixel->red(), other->red()));
    pixel->green(op(pixel->green(), other->green()));
    pixel->blue(op(pixel->blue(), other->blue()));
    // we don't modify alpha here
}

template <typename T>
void apply(PixelPtr<T> &pixel, T value, std::function<T(T, T)> op) {
    pixel->red(op(pixel->red(), value));
    pixel->green(op(pixel->green(), value));
    pixel->blue(op(pixel->blue(), value));
    // we don't modify alpha here
}

#define PixelOperator(Op)                                                      \
    template <typename T, typename RhsType>                                    \
    PixelPtr<T> const &operator Op(PixelPtr<T> &pixel, RhsType rhs) {          \
        auto op = [](T a, T b) { return a Op b; };                             \
        apply(pixel, rhs, op);                                                 \
        return pixel;                                                          \
    }
PixelOperator(+) PixelOperator(-) PixelOperator(*) PixelOperator(/)
#undef PixelOperator

} // namespace pixelOperators

} // namespace hhimg

#endif
