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

    // operator=
    AbstractPixel<T> const &
    operator=(std::shared_ptr<AbstractPixel<T>> const &other) {
        this->set(other);
        return *this;
    }

    AbstractPixel<T> const &operator=(AbstractPixel<T> const &other) {
        this->set(other);
        return *this;
    }

    // function for improving readability when working with gray images
    // (warn: we do not test if the pixel is actually gray)
    T get() const { return red(); }
};

} // namespace hhimg

/******************************************************************************/
/*                                 operators                                  */
/******************************************************************************/

template <typename T> using PixelPtr = std::shared_ptr<hhimg::AbstractPixel<T>>;

template <typename T>
void apply(PixelPtr<T> pixel, PixelPtr<T> other, std::function<T(T, T)> op) {
    pixel->red(op(pixel->red(), other->red()));
    pixel->green(op(pixel->green(), other->green()));
    pixel->blue(op(pixel->blue(), other->blue()));
    // we don't modify alpha here
}

template <typename T>
void apply(PixelPtr<T> pixel, T value, std::function<T(T, T)> op) {
    pixel->red(op(pixel->red(), value));
    pixel->green(op(pixel->green(), value));
    pixel->blue(op(pixel->blue(), value));
    // we don't modify alpha here
}

#define PixelAffectationOperator(Op)                                           \
    template <typename T, typename RhsType>                                    \
    PixelPtr<T> operator Op##=(PixelPtr<T> pixel, RhsType rhs) {               \
        std::function<T(T, T)> op = [](T a, T b) { return a Op b; };           \
        apply(pixel, rhs, op);                                                 \
        return pixel;                                                          \
    }
PixelAffectationOperator(+) PixelAffectationOperator(-)
    PixelAffectationOperator(*) PixelAffectationOperator(/)
#undef PixelAffectationOperator

#endif
