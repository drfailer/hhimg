#ifndef CIMG_TILE_FACTORY_HPP
#define CIMG_TILE_FACTORY_HPP
#include "cimg_image.h"
#include "hhimg/abstract/abstract_tile_factory.h"
#include "hhimg/abstract/data/abstract_image.h"
#include "hhimg/concrete/data/pixel_tile.h"

template <typename T> struct CImgTileFactory : hhimg::AbstractTileFactory<T> {
    std::shared_ptr<hhimg::PixelTile<T>>
    get(size_t x, size_t y, size_t ghostRegionSize) const override;

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
