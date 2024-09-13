#ifndef CIMG_TILE_HPP
#define CIMG_TILE_HPP
#include "impl/cimg/cimg_image.h"
#include <hhimg/hhimg.h>

template <typename T> class CImgTile : public hhimg::AbstractTile<T> {
  public:
    CImgTile(size_t x, size_t y, size_t tileSize,
             std::shared_ptr<CImgImage<T>> image)
        : hhimg::AbstractTile<T>(x, y, tileSize, image),
          dataRed_(&image->image()._data[y * image->width() + x]),
          dataBlue_(&image->image()._data[y * image->width() + x +
                                          image->width() * image->height() *
                                              image->image().depth()]),
          dataGreen_(
              &image->image()._data[y * image->width() + x +
                                    2 * image->width() * image->height() *
                                        image->image().depth()]),
          image_(image) {}

    hhimg::Pixel<T> at(size_t offset) const override {
        return {dataRed_[offset], dataGreen_[offset], dataBlue_[offset]};
    }

    void set(size_t offset, hhimg::Pixel<T> const &pixel) override {
        dataRed_[offset] = pixel.red;
        dataGreen_[offset] = pixel.green;
        dataBlue_[offset] = pixel.blue;
    }

    std::shared_ptr<hhimg::AbstractImage<T>> image() const override {
        return image_;
    }

  private:
    T *dataRed_ = nullptr;
    T *dataBlue_ = nullptr;
    T *dataGreen_ = nullptr;
    std::shared_ptr<CImgImage<T>> image_ = nullptr;
};

#endif
