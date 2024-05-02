#ifndef ABASTRACT_PIXEL_HPP
#define ABASTRACT_PIXEL_HPP
#include <functional>

template <typename T>
class AbstractPixel {
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
    void set(AbstractPixel<T> const &other) {
        set(other.red(), other.green(), other.blue(), other.alpha());
    }

    /* operators **************************************************************/

    AbstractPixel<T> operator=(AbstractPixel<T> const &other) {
        set(other.red(), other.green(), other.blue(), other.alpha());
        return *this;
    }

    AbstractPixel<T> operator+(AbstractPixel<T> const &other) {
        auto add = [this](T a, T b) { return validate(a + b); };
        apply(other, add);
        return *this;
    }

    AbstractPixel<T> operator-(AbstractPixel<T> const &other) {
        auto sub = [this](T a, T b) { validate(a - b); };
        apply(other, sub);
        return *this;
    }

    AbstractPixel<T> operator*(AbstractPixel<T> const &other) {
        auto div = [this](T a, T b) { return validate(a * b); };
        apply(other, div);
        return *this;
    }

    AbstractPixel<T> operator/(AbstractPixel<T> const &other) {
        auto div = [](T a, T b) { return a / b; }; // we suppose b > 0
        apply(other, div);
        return *this;
    }

  protected:
    void apply(AbstractPixel<T> const &other, std::function<T(T, T)>op) {
        this->red(op(this->red(), other.red()));
        this->green(op(this->green(), other.green()));
        this->blue(op(this->blue(), other.blue()));
        // this->alpha(op(this->alpha, other.alpha)); // don't touch the alpha here
    }

    T validate(T value) {
        if (value < 0)
            return 0;
        if (value > 255)
            return 255;
        return value;
    }
};

#endif
