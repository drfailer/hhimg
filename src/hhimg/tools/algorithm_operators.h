#ifndef ALGORITHM_OPERATORS_HPP
#define ALGORITHM_OPERATORS_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_hh_algorithm.h"
#include "../abstract/abstract_pair_tile_algorithm.h"
#include "../abstract/abstract_tile_algorithm.h"
#include "../algorithm/tile/ghost_region_state.h"
#include "../algorithm/tile/make_pair.h"
#include "../algorithm/tile/split.h"
#include "../concrete/chain_algorithm.h"
#include "hhimg/algorithm/tile/wire.h"
#include <memory>
#include <type_traits>

/******************************************************************************/
/*                                   images                                   */
/******************************************************************************/

// TODO: if possible use a constraint to check if Img derive from AbstractImage
template <typename Img, typename T>
std::shared_ptr<Img> operator|=(std::shared_ptr<Img> image,
                                hhimg::AbstractAlgorithm<T> const &algorithm) {
    image->set(algorithm(image));
    return image;
}

template <typename T>
hhimg::ChainAlgorithm<T>
operator|(hhimg::AbstractAlgorithm<T> const &algorithm1,
          hhimg::AbstractAlgorithm<T> const &algorithm2) {
    return hhimg::ChainAlgorithm<T>(algorithm1, algorithm2);
}

/******************************************************************************/
/*                                   tiles                                    */
/******************************************************************************/

template <typename Img, typename T>
std::shared_ptr<Img>
operator|=(std::shared_ptr<Img> image,
           std::shared_ptr<hhimg::AbstractHHAlgorithm<T>> algorithm) {
    return std::dynamic_pointer_cast<Img>(algorithm->operator()(image));
}

template <typename T>
std::shared_ptr<hhimg::AbstractHHAlgorithm<T>>
operator|(std::shared_ptr<hhimg::Split<T>> split,
          std::shared_ptr<hhimg::AbstractTileAlgorithm<T>> algorithm) {
    if (split->ghostRegionSize() < algorithm->ghostRegionSize()) {
        split->ghostRegionSize(algorithm->ghostRegionSize());
    }
    split->push_back(algorithm);
    return split;
}

template <typename T>
std::shared_ptr<hhimg::AbstractHHAlgorithm<T>>
operator|(std::shared_ptr<hhimg::AbstractHHAlgorithm<T>> hhAlgo,
          std::shared_ptr<hhimg::AbstractTileAlgorithm<T>> algorithm) {
    if (hhAlgo->ghostRegionSize() < algorithm->ghostRegionSize()) {
        hhAlgo->ghostRegionSize(algorithm->ghostRegionSize());
    }
    hhAlgo->push_back(algorithm);
    return hhAlgo;
}

template <typename T>
std::shared_ptr<hhimg::AbstractHHAlgorithm<T>>
operator|(std::shared_ptr<hhimg::AbstractHHAlgorithm<T>> hhAlgo,
          std::shared_ptr<hhimg::AbstractPairTileAlgorithm<T>> algorithm) {
    if (hhAlgo->ghostRegionSize() < algorithm->ghostRegionSize()) {
        hhAlgo->ghostRegionSize(algorithm->ghostRegionSize());
    }
    auto makePairTask =
        std::make_shared<hhimg::MakePair<T>>(algorithm->numberThreads());
    auto ghostRegionState = std::make_shared<hhimg::SyncGhostRegions<T>>();
    auto ghostRegionsStateManager = std::make_shared<
        hh::StateManager<1, std::pair<hhimg::Tile<T>, hhimg::Tile<T>>,
                         std::pair<hhimg::Tile<T>, hhimg::Tile<T>>>>(
        ghostRegionState);
    auto wire = std::make_shared<hhimg::Wire<T>>(algorithm->numberThreads());

    if (!hhAlgo->lastTask()) {
      hhAlgo->inputs(makePairTask);
    } else {
      hhAlgo->edges(hhAlgo->lastTask(), makePairTask);
    }
    hhAlgo->edges(makePairTask, ghostRegionsStateManager);
    hhAlgo->edges(ghostRegionsStateManager, algorithm);
    hhAlgo->edges(algorithm, wire);
    hhAlgo->lastTask(wire);
    return hhAlgo;
}

#endif
