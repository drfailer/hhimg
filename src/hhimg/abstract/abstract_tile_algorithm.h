#ifndef ABSTRACT_TILE_ALGORITHM_HPP
#define ABSTRACT_TILE_ALGORITHM_HPP
#include "../abstract/data/abstract_tile.h"
#include <functional>
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T> using Tile = std::shared_ptr<AbstractTile<T>>;
template <typename T>
using TaskType = hh::AbstractTask<1, AbstractTile<T>, AbstractTile<T>>;

template <typename T> struct AbstractTileAlgorithm : TaskType<T> {
    AbstractTileAlgorithm(std::string name = "tile algorithm",
                          size_t nbThreads = 1)
        : TaskType<T>(std::move(name), nbThreads) {}
    virtual ~AbstractTileAlgorithm() {}

    void execute(Tile<T> tile) override {
        this->operator()(tile);
    }

    virtual void operator()(Tile<T> tile) = 0;
};

} // namespace hhimg

#endif
