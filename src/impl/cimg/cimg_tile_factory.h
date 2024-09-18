#ifndef CIMG_TILE_FACTORY_HPP
#define CIMG_TILE_FACTORY_HPP
#include "cimg_image.h"
#include "cimg_tile.h"
#include "hhimg/abstract/abstract_tile_factory.h"

template <typename T>
class CImgTileFactory : public hhimg::AbstractTileFactory<T> {
  public:
    std::shared_ptr<hhimg::AbstractTile<T>> get(size_t x,
                                                size_t y,
                                                size_t ghostRegionSize) const override {
        return std::make_shared<CImgTile<T>>(x, y, this->tileSize_, ghostRegionSize, image_);
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
