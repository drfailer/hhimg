#ifndef MAKE_PAIR_H
#define MAKE_PAIR_H
#include "../../abstract/abstract_tile_algorithm.h"
#include "../../abstract/abstract_tile_factory.h"
#include "../../concrete/data/pixel_tile.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
using MakePairTaskType =
    hh::AbstractTask<1, AbstractTile<T>, std::pair<Tile<T>, Tile<T>>>;

template <typename T> struct MakePair : MakePairTaskType<T> {
    MakePair(size_t nbThreads) : MakePairTaskType<T>("MakePair", nbThreads) {}
    ~MakePair() = default;

    void execute(Tile<T> tile) override {
        auto newTile = std::make_shared<PixelTile<T>>(
            tile->x(), tile->y(), tile->tileSize(), tile->ghostRegionSize(),
            tile->image());

        newTile->copy(tile);
        this->addResult(
            std::make_shared<std::pair<Tile<T>, Tile<T>>>(tile, newTile));
    }

    std::shared_ptr<MakePairTaskType<T>> copy() override {
        return std::make_shared<MakePair<T>>(this->numberThreads());
    }

    std::shared_ptr<AbstractTileFactory<T>> tileFactory = nullptr;
    size_t tileSize = 0;
};

} // end namespace hhimg

#endif
