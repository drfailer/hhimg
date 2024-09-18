#ifndef ABSTRACT_PAIR_TILE_ALGORITHM_HPP
#define ABSTRACT_PAIR_TILE_ALGORITHM_HPP
#include "../abstract/data/abstract_tile.h"
#include "abstract_tile_algorithm.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
using PairTaskType =
    hh::AbstractTask<1, std::pair<Tile<T>, Tile<T>>, AbstractTile<T>>;

template <typename T> struct AbstractPairTileAlgorithm : PairTaskType<T> {
    AbstractPairTileAlgorithm(std::string name = "pair tile algorithm",
                              size_t nbThreads = 1)
        : PairTaskType<T>(std::move(name), nbThreads) {}
    virtual ~AbstractPairTileAlgorithm() {}

    void execute(std::shared_ptr<std::pair<Tile<T>, Tile<T>>> tiles) override {
        this->operator()(tiles);
    }

    virtual void
    operator()(std::shared_ptr<std::pair<Tile<T>, Tile<T>>> tiles) = 0;

    // can be overriden if a ghost region is required
    virtual size_t ghostRegionSize() const { return 0; }
};

} // namespace hhimg

#endif
