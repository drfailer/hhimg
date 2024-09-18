#ifndef CIMG_TILE_HPP
#define CIMG_TILE_HPP
#include "hhimg/abstract/data/abstract_tile.h"
#include "impl/cimg/cimg_image.h"

template <typename T> class CImgTile : public hhimg::AbstractTile<T> {
  public:
    CImgTile(size_t x, size_t y, size_t tileSize, size_t ghostRegionSize,
             std::shared_ptr<CImgImage<T>> image);

    using hhimg::AbstractTile<T>::set;
    using hhimg::AbstractTile<T>::at;

    hhimg::Pixel<T> at(size_t offset) const override;
    void set(size_t offset, hhimg::Pixel<T> const &pixel) override;

    hhimg::Pixel<T> ghostAt(size_t) const override;
    void ghostSet(size_t, hhimg::Pixel<T> const &) override;

    size_t fullWidth() const override { return this->image()->width(); }

  private:
    T *dataRed_ = nullptr;
    T *dataBlue_ = nullptr;
    T *dataGreen_ = nullptr;
};

#endif
