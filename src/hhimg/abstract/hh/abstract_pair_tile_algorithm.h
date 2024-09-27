#ifndef ABSTRACT_PAIR_TILE_ALGORITHM_H
#define ABSTRACT_PAIR_TILE_ALGORITHM_H
#include "../data/abstract_tile.h"
#include "abstract_hh_algorithm.h"

namespace hhimg {

template <typename T>
using PairTile = std::pair<std::shared_ptr<AbstractTile<T>>, std::shared_ptr<AbstractTile<T>>>;

template <typename T>
struct AbstractPairTileAlgorithm
    : AbstractHHAlgorithm<PairTile<T>, AbstractTile<T>> {
    using AbstractHHAlgorithm<PairTile<T>,
                              AbstractTile<T>>::AbstractHHAlgorithm;
    using TaskType =
        typename AbstractHHAlgorithm<PairTile<T>, AbstractTile<T>>::TaskType;
    using PixelType = T;
};

} // namespace hhimg

#endif
