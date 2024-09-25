#ifndef ABSTRACT_TILE_FACTORY_HPP
#define ABSTRACT_TILE_FACTORY_HPP
#include "../concrete/data/pixel_tile.h"
#include <cstddef>
#include <memory>

namespace hhimg {

template <typename T> class AbstractTileFactory {
  public:
    AbstractTileFactory() = default;
    virtual ~AbstractTileFactory() = default;

    virtual std::shared_ptr<PixelTile<T>>
    get(size_t x, size_t y, size_t tileSize, size_t ghostRegionSize,
        std::shared_ptr<hhimg::AbstractImage<T>> image) const = 0;

    virtual void copy(std::shared_ptr<AbstractTile<T>> tile) const = 0;

    size_t nbTileRows(size_t imageHeight, size_t tileSize) const {
        return imageHeight / tileSize + (imageHeight % tileSize == 0 ? 0 : 1);
    }

    size_t nbTileColumns(size_t imageWidth, size_t tileSize) const {
        return imageWidth / tileSize + (imageWidth % tileSize == 0 ? 0 : 1);
    }
};

} // namespace hhimg

#endif
