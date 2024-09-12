#ifndef ABSTRACT_TILE_ALGORITHM_HPP
#define ABSTRACT_TILE_ALGORITHM_HPP
#include "../abstract/data/abstract_tile.h"
#include <functional>
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T> using Tile = std::shared_ptr<AbstractTile<T>>;

template <typename T>
struct AbstractTileAlgorithm
    : hh::AbstractTask<1, AbstractTile<T>, AbstractTile<T>> {
    AbstractTileAlgorithm()
        : hh::AbstractTask<1, AbstractTile<T>, AbstractTile<T>>("test", 1) {}
    AbstractTileAlgorithm(AbstractTileAlgorithm<T> &)
        : hh::AbstractTask<1, AbstractTile<T>, AbstractTile<T>>("test", 1) {}
    virtual ~AbstractTileAlgorithm() {}

    void execute(Tile<T> tile) override {
        this->operator()(tile, [this](Tile<T> tile) { this->addResult(tile); });
    }

    virtual void operator()(Tile<T> tile,
                            std::function<void(Tile<T>)> addResult) const = 0;

    // virtual pure copy ?
};

} // namespace hhimg

#endif
