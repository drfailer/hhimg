#ifndef ABSTRACT_TILE_HPP
#define ABSTRACT_TILE_HPP
#include "abstract_image.h"
#include "abstract_pixel_container.h"
#include <memory>

namespace hhimg {

template <typename T> class AbstractTile : public AbstractPixelContainer<T> {
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
    size_t width() const override { return width_; }
    size_t fullWidth() const override { return this->image()->width(); }
    size_t height() const override { return height_; }
    size_t tileSize() const { return tileSize_; }

    using AbstractPixelContainer<T>::set;
    using AbstractPixelContainer<T>::at;

  protected:
    size_t x_ = 0;
    size_t y_ = 0;
    size_t width_ = 0;
    size_t height_ = 0;
    size_t tileSize_ = 0;
};

} // namespace hhimg

#endif
