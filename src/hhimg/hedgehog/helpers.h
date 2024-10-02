#ifndef HHIMG_HH_HELPERS_H
#define HHIMG_HH_HELPERS_H
#include "algorithm/convolution.h"
#include "algorithm/gray_scale.h"
#include "algorithm/map_mutate.h"
#include "algorithm/minus.h"
#include "algorithm/threshold.h"
#include "concrete/hedgehog_pipeline.h"
#include <memory>

namespace hhimg::hdg {

template <typename PixelType>
inline auto
pipeline(size_t tileSize, size_t splitThreads, size_t copyThreads,
         std::shared_ptr<AbstractTileFactory<PixelType>> tileFactory,
         std::string const &graphName = "unamed pipeline") {
    return std::make_shared<HedgehogPipeline<PixelType>>(
        tileSize, splitThreads, copyThreads, tileFactory, graphName);
}

template <typename PixelType>
inline auto subpipeline(std::string const &graphName = "unamed pipeline") {
    return std::make_shared<HedgehogPipeline<PixelType>>(0, 0, 0, nullptr,
                                                         graphName);
}

template <typename PixelType, typename MaskType>
inline auto convolution(size_t nbThreads, Mask<MaskType> mask) {
    return std::make_shared<Convolution<PixelType, double>>(nbThreads, mask);
}

template <typename PixelType, typename F = NullType, typename L = NullType>
inline auto
minus(size_t nbThreads, Image<PixelType> const &image,
      std::shared_ptr<HedgehogPipeline<PixelType, F, L>> pipeline = nullptr) {
    return std::make_shared<Minus<PixelType, F, L>>(nbThreads, image, pipeline);
}

template <typename PixelType> inline auto grayscale(size_t nbThreads) {
    return std::make_shared<GrayScale<PixelType>>(nbThreads);
}

template <typename PixelType>
inline auto threshold(size_t nbThreads, PixelType max, PixelType low = 0,
                      PixelType high = 255) {
    return std::make_shared<Threshold<PixelType>>(nbThreads, max, low, high);
}

template <typename PixelType>
inline auto mapMutate(size_t nbThreads,
                      typename MapMutate<PixelType>::ComputePixel compute) {
    return std::make_shared<MapMutate<PixelType>>(nbThreads, compute);
}

} // end namespace hhimg::hdg

#endif
