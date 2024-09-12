#ifndef CIMG_TILE_HPP
#define CIMG_TILE_HPP
#include "impl/cimg/cimg_image.h"
#include <hhimg/hhimg.h>

template <typename T> class CImgTile : public hhimg::AbstractTile<T> {
  public:
    CImgTile(size_t x, size_t y, size_t tileSize,
             std::shared_ptr<CImgImage<T>> image)
        : hhimg::AbstractTile<T>(x, y, tileSize, image),
          data_(&image->image()._data[y * image->width() + x]), image_(image) {}

    hhimg::Pixel<T> at(size_t offset) const override {
        return {data_[offset],
                data_[offset + image_->width() * image_->height() *
                                   image_->image().depth()],
                data_[offset + 2 * image_->width() * image_->height() *
                                   image_->image().depth()]};
    }

    void set(size_t offset, hhimg::Pixel<T> const &pixel) override {
        data_[offset] = pixel.red;
        data_[offset + image_->width() * image_->height() *
                           image_->image().depth()] = pixel.green;
        data_[offset + 2 * image_->width() * image_->height() *
                           image_->image().depth()] = pixel.blue;
    }

    std::shared_ptr<hhimg::AbstractImage<T>> image() const override {
        return image_;
    }

  private:
    T *data_ = nullptr;
    std::shared_ptr<CImgImage<T>> image_ = nullptr;
};

#endif
