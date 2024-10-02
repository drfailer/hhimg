#ifndef HHIMG_CREATE_TMP_TILES_H
#define HHIMG_CREATE_TMP_TILES_H
#include "../../abstract/abstract_tile_factory.h"
#include "../../abstract/tile_algorithms.h"
#include "../../concrete/data/pixel_tile.h"
#include <hedgehog/hedgehog.h>

namespace hhimg::hdg {

template <typename T>
using CreateTmpTilesType = hh::AbstractTask<1, AbstractTile<T>, PairTile<T>>;

template <typename T> struct CreateTmpTiles : CreateTmpTilesType<T> {
    CreateTmpTiles(size_t nbThreads)
        : CreateTmpTilesType<T>("Create Temporary Tiles", nbThreads) {}
    ~CreateTmpTiles() = default;

    void execute(std::shared_ptr<AbstractTile<T>> tile) override {
        auto newTile = std::make_shared<PixelTile<T>>(
            tile->x(), tile->y(), tile->tileSize(), tile->ghostRegionSize(),
            tile->image());

        this->addResult(std::make_shared<PairTile<T>>(newTile, tile));
    }

    std::shared_ptr<CreateTmpTilesType<T>> copy() override {
        return std::make_shared<CreateTmpTiles<T>>(this->numberThreads());
    }

    std::shared_ptr<AbstractTileFactory<T>> tileFactory = nullptr;
    size_t tileSize = 0;
};

} // end namespace hhimg::hdg

#endif
