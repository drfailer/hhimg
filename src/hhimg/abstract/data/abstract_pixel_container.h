#ifndef ABSTRACT_PIXEL_CONTAINER_H
#define ABSTRACT_PIXEL_CONTAINER_H
#include "../../concrete/data/pixel.h"

namespace hhimg {

template <typename T>
struct AbstractPixelContainer {
    virtual size_t width() const = 0;
    virtual size_t height() const = 0;
    virtual size_t fullWidth() const { return width(); }
    size_t size() const { return width() * height(); }

    // access to the image
    virtual Pixel<T> at(size_t offset) const = 0;
    virtual void set(size_t offset, Pixel<T> const &pixel) = 0;

    // acces with x,y coordinates (can be overrided for optimization)
    virtual Pixel<T> at(size_t x, size_t y) const {
        return at(y * fullWidth() + x);
    }
    virtual void set(size_t x, size_t y, Pixel<T> const &pixel) {
        set(y * fullWidth() + x, pixel);
    }

    // set without pixel
    virtual void set(size_t offset, T r, T g, T b) { set(offset, {r, g, b}); }
    virtual void set(size_t x, size_t y, T r, T g, T b) {
        set(y * fullWidth() + x, r, g, b);
    }

    // for grayscaled images
    void set(size_t offset, T v) { set(offset, v, v, v); }
    void set(size_t x, size_t y, T v) { set(x, y, v, v, v); }
    T get(size_t offset) const { return at(offset).red; }
    T get(size_t x, size_t y) const { return at(x, y).red; }

    // pixel type
    using PixelType = T;
};

} // end namespace hhimg

#endif
