#ifndef FLD_OPERATORS_H
#define FLD_OPERATORS_H
#include "../tools/concepts.h"

template <typename TileLoader, typename FastLoader>
void operator|=(hhimg::fld::FLImg<TileLoader> image, FastLoader pipeline) {
    pipeline->operator()(image);
}

template <hhimg::FLPipeline Pipeline, hhimg::FLAlgorithms Algo>
    requires(requires(Pipeline p, Algo a) {
        hhimg::clear_t<Algo>::setup(p, a);
    })
auto operator|(Pipeline pipeline, Algo algorithm) {
    if (pipeline->ghostRegionSize() < algorithm->ghostRegionSize()) {
        pipeline->ghostRegionSize(algorithm->ghostRegionSize());
    }
    return hhimg::clear_t<Algo>::setup(pipeline, algorithm);
}

template <hhimg::FLPipeline Pipeline, hhimg::FLAlgorithms Algo>
    requires(requires(Pipeline p, Algo a) {
        hhimg::clear_t<Algo>::setup(p, a);
    })
auto operator|(Algo algorithm, Pipeline pipeline) {
    return (pipeline | algorithm);
}

#endif
