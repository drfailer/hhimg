#ifndef HDG_MAP_MUTATE_HPP
#define HDG_MAP_MUTATE_HPP
#include "../abstract/tile_algorithms.h"
#include <functional>

namespace hhimg::hdg {

template <typename T> struct MapMutate : AbstractTileAlgorithm<T> {
    using ComputePixel = std::function<Pixel<T>(
        std::shared_ptr<AbstractPixelContainer<T>> const &, size_t, size_t)>;
    MapMutate(size_t nbThreads, ComputePixel compute)
        : AbstractTileAlgorithm<T>(nbThreads, "MapMutate"), compute_(compute) {}
    MapMutate(ComputePixel compute) : MapMutate<T>(1, compute) {}

    void operator()(std::shared_ptr<AbstractTile<T>> tile) override {
        for (size_t y = 0; y < tile->height(); ++y) {
            for (size_t x = 0; x < tile->width(); ++x) {
                tile->set(x, y, compute_(tile, x, y));
            }
        }
        this->addResult(tile);
    }

    std::shared_ptr<typename AbstractTileAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<MapMutate<T>>(this->numberThreads(), compute_);
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }

  private:
    ComputePixel compute_;
};

} // namespace hhimg::hdg

#endif
