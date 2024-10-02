#ifndef HDG_OPERATORS_H
#define HDG_OPERATORS_H
#include "../tools/concepts.h"

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

template <hhimg::HHPipeline Pipeline, hhimg::TileAlgorithms Algo>
    requires(requires(Pipeline p, Algo a) {
        hhimg::clear_t<Algo>::setup(p, a);
    })
auto operator|(Algo algorithm, Pipeline pipeline) {
    return (pipeline | algorithm);
}

#endif
