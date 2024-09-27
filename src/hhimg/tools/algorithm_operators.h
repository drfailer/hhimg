#ifndef ALGORITHM_OPERATORS_HPP
#define ALGORITHM_OPERATORS_HPP
#include "../abstract/abstract_algorithm.h"
#include "../algorithm/tile/tmp_tiles_graph.h"
#include "../concrete/chain_algorithm.h"
#include "../concrete/hedgehog_pipeline.h"
#include "hhimg/algorithm/tile/update_stencils_graph.h"
#include "hhimg/tools/concepts.h"
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

// todo: The hedgehog pipeline should take the type of the "lastTask" as
// template parameter.
// todo: operators should use constexpr instead of types (more flexible).
template <hhimg::HHPipeline Pipeline, hhimg::TileAlgorithms Algo>
Pipeline operator|(Pipeline pipeline, Algo algorithm) {
    using PixelType = hhimg::pixel_type_t<Algo>;
    if (pipeline->ghostRegionSize() < algorithm->ghostRegionSize()) {
        pipeline->ghostRegionSize(algorithm->ghostRegionSize());
    }
    if (algorithm->ghostRegionSize()) {
        pipeline->add(
            std::make_shared<hhimg::UpdateStencilsGraph<PixelType>>());
    }
    if constexpr (hhimg::PairTileAlgorithm<Algo>) {
        auto pairGraph =
            std::make_shared<hhimg::TmpTilesGraph<PixelType>>(algorithm);
        pipeline->add(pairGraph);
    } else {
        pipeline->add(algorithm);
    }
    return pipeline;
}

#endif
