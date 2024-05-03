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

    /* operators **************************************************************/

    std::shared_ptr<AbstractPixel<T>>
    operator=(std::shared_ptr<AbstractPixel<T>> const other) {
        set(other->red(), other->green(), other->blue(), other->alpha());
        return *this;
    }

    std::shared_ptr<AbstractPixel<T>>
    operator+(std::shared_ptr<AbstractPixel<T>> const other) {
        auto add = [this](T a, T b) { return a + b; };
        apply(other, add);
        return *this;
    }

    std::shared_ptr<AbstractPixel<T>>
    operator-(std::shared_ptr<AbstractPixel<T>> const other) {
        auto sub = [this](T a, T b) { return a - b; };
        apply(other, sub);
        return *this;
    }

    std::shared_ptr<AbstractPixel<T>>
    operator*(std::shared_ptr<AbstractPixel<T>> const other) {
        auto div = [this](T a, T b) { return a * b; };
        apply(other, div);
        return *this;
    }

    std::shared_ptr<AbstractPixel<T>>
    operator/(std::shared_ptr<AbstractPixel<T>> const other) {
        auto div = [](T a, T b) { return a / b; }; // we suppose b > 0
        apply(other, div);
        return *this;
    }

    std::shared_ptr<AbstractPixel<T>> operator+(T value) {
        auto add = [this](T a, T b) { return a + b; };
        apply(value, add);
        return *this;
    }

    std::shared_ptr<AbstractPixel<T>> operator-(T value) {
        auto sub = [this](T a, T b) { return a - b; };
        apply(value, sub);
        return *this;
    }

    std::shared_ptr<AbstractPixel<T>> operator*(T value) {
        auto div = [this](T a, T b) { return a * b; };
        apply(value, div);
        return *this;
    }

    std::shared_ptr<AbstractPixel<T>> operator/(T value) {
        auto div = [](T a, T b) { return a / b; }; // we suppose b > 0
        apply(value, div);
        return *this;
    }

  protected:
    void apply(std::shared_ptr<AbstractPixel<T>> const other,
               std::function<T(T, T)> op) {
        this->red(op(this->red(), other->red()));
        this->green(op(this->green(), other->green()));
        this->blue(op(this->blue(), other->blue()));
        // this->alpha(op(this->alpha, other->alpha())); // don't touch the
        // alpha here
    }

    void apply(T value, std::function<T(T, T)> op) {
        this->red(op(this->red(), value));
        this->green(op(this->green(), value));
        this->blue(op(this->blue(), value));
        // this->alpha(op(this->alpha, value)); // don't touch the alpha here
    }
};

} // namespace hhimg

#endif
