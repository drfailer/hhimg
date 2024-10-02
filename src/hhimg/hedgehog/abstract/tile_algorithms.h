#ifndef TILE_ALGORITHMS_H
#define TILE_ALGORITHMS_H
#include "data/abstract_tile.h"
#include "abstract_hh_algorithm.h"

namespace hhimg::hdg {

/* helper types ***************************************************************/

template <typename T>
using PairTile = std::pair<std::shared_ptr<AbstractTile<T>>,
                           std::shared_ptr<AbstractTile<T>>>;

/* algorithms types ***********************************************************/

template <typename T>
using AbstractTileAlgorithm =
    AbstractHHAlgorithm<AbstractTile<T>, AbstractTile<T>>;

template <typename T>
using AbstractTmpTileAlgorithm =
    AbstractHHAlgorithm<PairTile<T>, AbstractTile<T>>;

template <typename T>
using Abstract2TilesAlgorithm =
    AbstractHHAlgorithm<PairTile<T>, AbstractTile<T>>;

} // end namespace hhimg::hdg

#endif
