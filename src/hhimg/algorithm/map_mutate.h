#ifndef MAP_MUTATE_HPP
#define MAP_MUTATE_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_tile_algorithm.h"
#include <functional>

namespace hhimg {

template <typename T>
struct MapMutate : AbstractAlgorithm<T>, AbstractTileAlgorithm<T> {
    using ComputePixel = std::function<Pixel<T>(
        std::shared_ptr<AbstractPixelContainer<T>> const&, size_t, size_t)>;

    MapMutate(size_t nbThreads, ComputePixel compute)
        : AbstractTileAlgorithm<T>("MapMutate", nbThreads),
          compute_(compute) {}

    MapMutate(ComputePixel compute) : hhimg::MapMutate<T>(1, compute) {}

    void compute(std::shared_ptr<AbstractPixelContainer<T>> elt) const {
        for (size_t y = 0; y < elt->height(); ++y) {
            for (size_t x = 0; x < elt->width(); ++x) {
                elt->set(x, y, compute_(elt, x, y));
            }
        }
    }

    Image<T> operator()(Image<T> image) const override {
        compute(image);
        return image;
    }

    void operator()(Tile<T> tile) override {
        compute(tile);
        this->addResult(tile);
    }

    std::shared_ptr<TaskType<T>> copy() override {
        return std::make_shared<MapMutate<T>>(this->numberThreads(), compute_);
    }

  private:
    ComputePixel compute_;
};

} // namespace hhimg

#endif
