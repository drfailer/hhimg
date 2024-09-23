#ifndef ABSTRACT_PIXEL_CONTAINER_H
#define ABSTRACT_PIXEL_CONTAINER_H
#include "../../concrete/data/pixel.h"

namespace hhimg {

template <typename T> struct AbstractPixelContainer {
    virtual size_t width() const = 0;
    virtual size_t height() const = 0;
    virtual size_t fullWidth() const { return width(); }
    size_t size() const { return width() * height(); }

    // acces with x,y coordinates (can be overrided for optimization)
    virtual Pixel<T> at(signed int x, signed int y) const = 0;
    virtual void set(signed int x, signed int y, Pixel<T> const &pixel) = 0;

    // set without pixel
    virtual void set(signed int x, signed int y, T r, T g, T b) {
        set(x, y, {r, g, b});
    }

    // for grayscaled images
    void set(signed int x, signed int y, T v) { set(x, y, {v, v, v}); }
    T get(signed int x, signed int y) const { return at(x, y).red; }

    // pixel type
    using PixelType = T;
};

} // end namespace hhimg

#endif
