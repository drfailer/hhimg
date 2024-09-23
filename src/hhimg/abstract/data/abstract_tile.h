#ifndef ABSTRACT_TILE_HPP
#define ABSTRACT_TILE_HPP
#include "abstract_image.h"
#include "abstract_pixel_container.h"
#include <memory>

namespace hhimg {

template <typename T> class AbstractTile : public AbstractPixelContainer<T> {
  public:
    AbstractTile(size_t x, size_t y, size_t tileSize, size_t ghostRegionSize,
                 std::shared_ptr<AbstractImage<T>> image)
        : x_(x), y_(y), width_(std::min(tileSize, image->width() - x)),
          height_(std::min(tileSize, image->height() - y)), tileSize_(tileSize),
          padding_(ghostRegionSize),
          ghostX_(std::max((int)(x - ghostRegionSize), 0)),
          ghostY_(std::max((int)(y - ghostRegionSize), 0)),
          ghostWidth_(std::min(tileSize + 2 * ghostRegionSize,
                               image->width() - ghostX_)),
          ghostHeight_(std::min(tileSize + 2 * ghostRegionSize,
                                image->height() - ghostY_)),
          image_(image) {}

    // get the full image
    virtual std::shared_ptr<AbstractImage<T>> const image() const {
        return image_;
    }

    // tile position in the image
    size_t x() const { return x_; }
    size_t y() const { return y_; }

    // tile size
    size_t width() const override { return width_; }
    size_t height() const override { return height_; }
    size_t tileSize() const { return tileSize_; }

    using AbstractPixelContainer<T>::set;
    using AbstractPixelContainer<T>::at;

    Pixel<T> at(signed int x, signed int y) const override {
        y += ghostRegionSize();
        x += ghostRegionSize();
        return ghostAt(x, y);
    }
    void set(signed int x, signed int y, Pixel<T> const &pixel) override {
        y += ghostRegionSize();
        x += ghostRegionSize();
        ghostSet(x, y, pixel);
    }

    /* ghost tile *************************************************************/

    size_t ghostX() const { return ghostX_; }
    size_t ghostY() const { return ghostY_; }
    size_t ghostWidth() const { return ghostWidth_; }
    size_t ghostHeight() const { return ghostHeight_; }
    size_t ghostTileSize() const { return tileSize_ + 2 * padding_; }
    size_t ghostRegionSize() const { return padding_; }

    virtual Pixel<T> ghostAt(size_t x, size_t y) const = 0;
    virtual void ghostSet(size_t x, size_t y, Pixel<T> const &pixel) = 0;

    virtual void ghostSet(size_t x, size_t y, T r, T g, T b) {
        ghostSet(x, y, {r, g, b});
    }

    // for grayscaled images
    void ghostSet(size_t x, size_t y, T v) { ghostSet(x, y, v, v, v); }
    T ghostGet(size_t x, size_t y) const { return ghostAt(x, y).red; }

    /* copy *******************************************************************/

    virtual void copy(std::shared_ptr<AbstractTile<T>> other) {
        for (size_t y = 0; y < ghostHeight_; ++y) {
            for (size_t x = 0; x < ghostWidth_; ++x) {
                this->ghostSet(x, y, other->ghostGet(x, y));
            }
        }
    }

  protected:
    size_t x_ = 0;
    size_t y_ = 0;
    size_t width_ = 0;
    size_t height_ = 0;
    size_t tileSize_ = 0;
    size_t padding_ = 0;

    // ghost region
    size_t ghostX_ = 0;
    size_t ghostY_ = 0;
    size_t ghostWidth_ = 0;
    size_t ghostHeight_ = 0;

    // the image
    std::shared_ptr<AbstractImage<T>> image_ = nullptr;
};

} // namespace hhimg

#endif
