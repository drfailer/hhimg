#ifndef ABSTRACT_TILE_HPP
#define ABSTRACT_TILE_HPP
#include "../../concrete/data/pixel.h"
#include "abstract_image.h"
#include <cstddef>
#include <memory>

namespace hhimg {

template <typename T> class AbstractTile {
  public:
    AbstractTile(size_t x, size_t y, size_t tileSize,
                 std::shared_ptr<AbstractImage<T>> image)
        : x_(x), y_(y), width_(std::min(tileSize, image->width() - x)),
          height_(std::min(tileSize, image->height() - y)),
          tileSize_(tileSize) {}

    // get the full image
    virtual std::shared_ptr<AbstractImage<T>> image() const = 0;

    // tile position in the image
    size_t x() const { return x_; }
    size_t y() const { return y_; }

    // tile size
    size_t width() const { return width_; }
    size_t height() const { return height_; }
    size_t tileSize() const { return tileSize_; }

    /* virtual void set(std::shared_ptr<AbstractTile<T>> &&other) = 0; */

    // access to the image
    virtual Pixel<T> at(size_t offset) const = 0;
    virtual void set(size_t offset, Pixel<T> const &pixel) = 0;

    // acces with x,y coordinates (can be overrided for optimization)
    virtual Pixel<T> at(size_t x, size_t y) const {
        return at(y * this->image()->width() + x);
    }
    virtual void set(size_t x, size_t y, Pixel<T> const &pixel) {
        set(y * this->image()->width() + x, pixel);
    }

    // set without pixel
    virtual void set(size_t offset, T r, T g, T b) { set(offset, {r, g, b}); }
    virtual void set(size_t x, size_t y, T r, T g, T b) {
        set(y * this->image()->width() + x, r, g, b);
    }

    // for grayscaled images
    void set(size_t offset, T v) { set(offset, v, v, v); }
    void set(size_t x, size_t y, T v) { set(x, y, v, v, v); }
    T get(size_t offset) const { return at(offset).red; }
    T get(size_t x, size_t y) const { return at(x, y).red; }

    // pixel type
    using PixelType = T;

  protected:
    size_t x_ = 0;
    size_t y_ = 0;
    size_t width_ = 0;
    size_t height_ = 0;
    size_t tileSize_ = 0;
};

} // namespace hhimg

#endif
