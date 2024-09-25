#ifndef CIMG_TILE_FACTORY_HPP
#define CIMG_TILE_FACTORY_HPP
#include "hhimg/abstract/abstract_tile_factory.h"
#include "hhimg/concrete/data/pixel_tile.h"

template <typename T> struct CImgTileFactory : hhimg::AbstractTileFactory<T> {
    std::shared_ptr<hhimg::PixelTile<T>>
    get(size_t x, size_t y, size_t tileSize, size_t ghostRegionSize,
        std::shared_ptr<hhimg::AbstractImage<T>> image) const override;
    void copy(std::shared_ptr<hhimg::AbstractTile<T>> tile) const override;
};

#endif
