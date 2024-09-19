#ifndef CIMG_TILE_FACTORY_HPP
#define CIMG_TILE_FACTORY_HPP
#include "safe_cimg.h"
#include "cimg_image.h"
#include "cimg_tile.h"
#include "hhimg/abstract/abstract_tile_factory.h"
#include "hhimg/abstract/data/abstract_image.h"
#include "hhimg/abstract/data/abstract_tile.h"
#include "hhimg/concrete/data/pixel_tile.h"

template <typename T>
class CImgTileFactory : public hhimg::AbstractTileFactory<T> {
  public:
    std::shared_ptr<hhimg::AbstractTile<T>>
    get(size_t x, size_t y, size_t ghostRegionSize) const override {
        std::shared_ptr<hhimg::AbstractTile<T>> tile =
            std::make_shared<hhimg::PixelTile<T>>(x, y, this->tileSize_,
                                                  ghostRegionSize, image_);

        size_t offset = tile->ghostY() * tile->fullWidth() + tile->ghostX();
        size_t size =
            image_->width() * image_->height() * image_->image().depth();
        auto dataRed = &image_->image()._data[offset];
        auto dataGreen = &image_->image()._data[offset + size];
        auto dataBlue = &image_->image()._data[offset + 2 * size];

        for (size_t iy = 0; iy < tile->ghostHeight(); ++iy) {
            for (size_t ix = 0; ix < tile->ghostWidth(); ++ix) {
                tile->ghostSet(ix, iy,
                               hhimg::Pixel<T>{
                                   dataRed[ix + iy * image_->fullWidth()],
                                   dataGreen[ix + iy * image_->fullWidth()],
                                   dataBlue[ix + iy * image_->fullWidth()],
                               });
            }
        }
        return tile;
    }

    std::shared_ptr<hhimg::AbstractImage<T>> image() const override {
        return image_;
    }
    void image(std::shared_ptr<hhimg::AbstractImage<T>> image) override {
        image_ = std::dynamic_pointer_cast<CImgImage<T>>(image);
    }

  private:
    std::shared_ptr<CImgImage<T>> image_ = nullptr;
};

#endif
