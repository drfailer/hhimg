#ifndef SEQ_MAP_MUTATE_HPP
#define SEQ_MAP_MUTATE_HPP
#include "../abstract/abstract_algorithm.h"
#include <functional>

namespace hhimg::seq {

template <typename T>
struct MapMutate : AbstractAlgorithm<T> {
    using ComputePixel = std::function<Pixel<T>(
        std::shared_ptr<AbstractPixelContainer<T>> const &, size_t, size_t)>;
    MapMutate(ComputePixel compute) : compute_(compute) {}

    Image<T> operator()(Image<T> image) const override {
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                image->set(x, y, compute_(image, x, y));
            }
        }
        return image;
    }

  private:
    ComputePixel compute_;
};

} // namespace hhimg::seq

#endif
