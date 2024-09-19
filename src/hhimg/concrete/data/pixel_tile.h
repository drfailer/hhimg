#ifndef HHIMG_PIXEL_TILE_H
#define HHIMG_PIXEL_TILE_H
#include "../../abstract/data/abstract_tile.h"
#include <memory>

namespace hhimg {

template <typename T> class PixelTile : public AbstractTile<T> {
  public:
    PixelTile(size_t x, size_t y, size_t tileSize, size_t ghostRegionSize,
              std::shared_ptr<AbstractImage<T>> image)
        : AbstractTile<T>(x, y, tileSize, ghostRegionSize, image),
          data_(new Pixel<T>[this->ghostWidth() * this->ghostHeight()]) {
        for (size_t i = 0; i < this->ghostHeight(); ++i) {
            for (size_t j = 0; j < this->ghostWidth(); ++j) {
                data_[i * this->ghostWidth() + j] = image->at(x + j, y + i);
            }
        }
    }
    ~PixelTile() { delete[] data_; }

    size_t fullWidth() const override { return this->ghostWidth_; }

    Pixel<T> at(size_t offset) const override { return data_[offset]; }
    void set(size_t offset, Pixel<T> const &pixel) override {
        data_[offset] = pixel;
    }

    Pixel<T> ghostAt(size_t offset) const override { return data_[offset]; }
    void ghostSet(size_t offset, Pixel<T> const &pixel) override {
        data_[offset] = pixel;
    }

    using AbstractTile<T>::at;
    using AbstractTile<T>::set;
    using AbstractTile<T>::ghostAt;
    using AbstractTile<T>::ghostSet;

  private:
    Pixel<T> *data_ = nullptr;
};

} // end namespace hhimg

#endif
