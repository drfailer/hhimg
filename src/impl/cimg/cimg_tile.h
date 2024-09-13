#ifndef CIMG_TILE_HPP
#define CIMG_TILE_HPP
#include "impl/cimg/cimg_image.h"
#include <hhimg/hhimg.h>

template <typename T> class CImgTile : public hhimg::AbstractTile<T> {
  public:
    CImgTile(size_t x, size_t y, size_t tileSize,
             std::shared_ptr<CImgImage<T>> image);

    hhimg::Pixel<T> at(size_t offset) const override;
    void set(size_t offset, hhimg::Pixel<T> const &pixel) override;

    std::shared_ptr<hhimg::AbstractImage<T>> image() const override;

  private:
    T *dataRed_ = nullptr;
    T *dataBlue_ = nullptr;
    T *dataGreen_ = nullptr;
    std::shared_ptr<CImgImage<T>> image_ = nullptr;
};

#endif
