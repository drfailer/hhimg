#ifndef ABSTRACT_TILE_FACTORY_HPP
#define ABSTRACT_TILE_FACTORY_HPP
#include "./data/abstract_tile.h"
#include <cstddef>
#include <memory>

namespace hhimg {

// TODO: ghost region !!!
template <typename T> class AbstractTileFactory {
  public:
    AbstractTileFactory() = default;
    virtual ~AbstractTileFactory() = default;

    // the factory will store the tile to compute the number of tiles on the row
    // and columns
    size_t tileSize() const { return tileSize_; }
    void tileSize(size_t tileSize) { this->tileSize_ = tileSize; }

    // the factory should be able to store the image
    virtual std::shared_ptr<AbstractImage<T>> image() const = 0;
    virtual void image(std::shared_ptr<AbstractImage<T>> image) = 0;

    virtual std::shared_ptr<AbstractTile<T>>
    get(size_t x, size_t y, size_t ghostRegionSize = 0) const = 0;

    size_t nbTileRows() const {
        return image()->height() / tileSize_ +
               (image()->height() % tileSize_ == 0 ? 0 : 1);
    }

    size_t nbTileColumns() const {
        return image()->width() / tileSize_ +
               (image()->width() % tileSize_ == 0 ? 0 : 1);
    }

  protected:
    size_t tileSize_ = 0;
};

} // namespace hhimg

#endif
