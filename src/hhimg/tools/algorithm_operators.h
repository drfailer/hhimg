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

template <typename Img, hhimg::HHPipeline Pipeline>
std::shared_ptr<Img> operator|=(std::shared_ptr<Img> image, Pipeline pipeline) {
    return std::dynamic_pointer_cast<Img>(pipeline->operator()(image));
}

template <hhimg::HHPipeline Pipeline, hhimg::TileAlgorithms Algo>
    requires(requires(Pipeline p, Algo a) {
        hhimg::clear_t<Algo>::setup(p, a);
    })
auto operator|(Pipeline pipeline, Algo algorithm) {
    if (pipeline->ghostRegionSize() < algorithm->ghostRegionSize()) {
        pipeline->ghostRegionSize(algorithm->ghostRegionSize());
    }
    return hhimg::clear_t<Algo>::setup(pipeline, algorithm);
}

#endif
