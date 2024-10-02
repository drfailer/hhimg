#ifndef CIMG_TILE_FACTORY_HPP
#define CIMG_TILE_FACTORY_HPP
#include "hhimg/hedgehog/abstract/abstract_tile_factory.h"
#include "hhimg/hedgehog/concrete/data/pixel_tile.h"

template <typename T> struct CImgTileFactory : hhimg::hdg::AbstractTileFactory<T> {
    std::shared_ptr<hhimg::hdg::PixelTile<T>>
    get(size_t x, size_t y, size_t tileSize, size_t ghostRegionSize,
        std::shared_ptr<hhimg::AbstractImage<T>> image) const override;
    void copy(std::shared_ptr<hhimg::hdg::AbstractTile<T>> tile) const override;
};

#endif
