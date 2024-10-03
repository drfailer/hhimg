#ifndef FLD_OPERATORS_H
#define FLD_OPERATORS_H
#include "../tools/concepts.h"

template <typename Img, typename FastLoader>
    requires(requires(Img i) {
        i.tileLoader;
        i.tileWriter;
    })
void operator|=(Img image, FastLoader pipeline) {
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
