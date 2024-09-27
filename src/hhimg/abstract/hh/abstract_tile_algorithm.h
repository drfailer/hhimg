#ifndef ABSTRACT_TILE_ALGORITHM_H
#define ABSTRACT_TILE_ALGORITHM_H
#include "../data/abstract_tile.h"
#include "abstract_hh_algorithm.h"

namespace hhimg {

template <typename T>
struct AbstractTileAlgorithm
    : AbstractHHAlgorithm<AbstractTile<T>, AbstractTile<T>> {
    using AbstractHHAlgorithm<AbstractTile<T>,
                              AbstractTile<T>>::AbstractHHAlgorithm;
    using TaskType = typename AbstractHHAlgorithm<AbstractTile<T>, AbstractTile<T>>::TaskType;
    using PixelType = T;
};

} // namespace hhimg

#endif
