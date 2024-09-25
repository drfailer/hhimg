#ifndef ALGORITHM_OPERATORS_HPP
#define ALGORITHM_OPERATORS_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/hh/abstract_pair_tile_algorithm.h"
#include "../abstract/hh/abstract_tile_algorithm.h"
#include "../algorithm/tile/tmp_tiles_graph.h"
#include "../concrete/chain_algorithm.h"
#include "../concrete/hedgehog_pipeline.h"
#include "hhimg/algorithm/tile/update_stencils_graph.h"
#include <memory>

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
           std::shared_ptr<hhimg::HedgehogPipeline<T>> algorithm) {
    return std::dynamic_pointer_cast<Img>(algorithm->operator()(image));
}

template <typename T>
std::shared_ptr<hhimg::HedgehogPipeline<T>>
operator|(std::shared_ptr<hhimg::HedgehogPipeline<T>> hhAlgo,
          std::shared_ptr<hhimg::AbstractTileAlgorithm<T>> algorithm) {
    if (hhAlgo->ghostRegionSize() < algorithm->ghostRegionSize()) {
        hhAlgo->ghostRegionSize(algorithm->ghostRegionSize());
    }
    if (algorithm->ghostRegionSize()) {
      hhAlgo->push_back(std::make_shared<hhimg::UpdateStencilsGraph<T>>());
    }
    hhAlgo->push_back(algorithm);
    return hhAlgo;
}

template <typename T>
std::shared_ptr<hhimg::HedgehogPipeline<T>>
operator|(std::shared_ptr<hhimg::HedgehogPipeline<T>> hhAlgo,
          std::shared_ptr<hhimg::AbstractPairTileAlgorithm<T>> algorithm) {
    if (hhAlgo->ghostRegionSize() < algorithm->ghostRegionSize()) {
        hhAlgo->ghostRegionSize(algorithm->ghostRegionSize());
    }
    if (algorithm->ghostRegionSize()) {
      hhAlgo->push_back(std::make_shared<hhimg::UpdateStencilsGraph<T>>());
    }
    auto pairGraph = std::make_shared<hhimg::TmpTilesGraph<T>>(algorithm);
    hhAlgo->push_back(pairGraph);
    return hhAlgo;
}

#endif
