#ifndef HHIMG_PIXEL_TILE_H
#define HHIMG_PIXEL_TILE_H
#include "../../../common/abstract_image.h"
#include "../../abstract/data/abstract_tile.h"
#include <cstring>
#include <memory>

namespace hhimg::hdg {

template <typename T> class PixelTile : public AbstractTile<T> {
  public:
    PixelTile(size_t x, size_t y, size_t tileSize, size_t ghostRegionSize,
              std::shared_ptr<AbstractImage<T>> image)
        : AbstractTile<T>(x, y, tileSize, ghostRegionSize, image),
          data_(new Pixel<T>[this->ghostTileSize() * this->ghostTileSize()]) {}
    ~PixelTile() { delete[] data_; }

    size_t fullWidth() const override { return this->ghostWidth_; }

    Pixel<T> ghostAt(size_t x, size_t y) const override {
        return data_[y * fullWidth() + x];
    }
    void ghostSet(size_t x, size_t y, Pixel<T> const &pixel) override {
        data_[y * fullWidth() + x] = pixel;
    }

    using AbstractTile<T>::at;
    using AbstractTile<T>::set;
    using AbstractTile<T>::ghostAt;
    using AbstractTile<T>::ghostSet;

    Pixel<T> *data() { return data_; }

    void copy(std::shared_ptr<AbstractTile<T>> other) override {
        if (auto pixelTile = std::dynamic_pointer_cast<PixelTile<T>>(other)) {
            size_t size =
                sizeof(Pixel<T>) * this->ghostWidth() * this->ghostHeight();
            memcpy(data_, pixelTile->data(), size);
        } else {
            this->template AbstractTile<T>::copy(other);
        }
    }

  private:
    Pixel<T> *data_ = nullptr;
};

} // end namespace hhimg::hdg

#endif
