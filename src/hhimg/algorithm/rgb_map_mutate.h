#ifndef RGB_MAP_MUTATE_HPP
#define RGB_MAP_MUTATE_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_tile_algorithm.h"
#include <functional>

namespace hhimg {

template <typename T>
struct RGBMapMutate : AbstractAlgorithm<T>, AbstractTileAlgorithm<T> {
    using ComputePixel = std::function<T(
        std::shared_ptr<AbstractPixelContainer<T>>, size_t, size_t)>;

    // for images
    RGBMapMutate(ComputePixel &&computeRed, ComputePixel &&computeGreen,
                 ComputePixel &&computeBlue)
        : computeRed_(computeRed), computeGreen_(computeGreen),
          computeBlue_(computeBlue) {}

    void compute(std::shared_ptr<AbstractPixelContainer<T>> elt) const {
        for (size_t y = 0; y < elt->height(); ++y) {
            for (size_t x = 0; x < elt->width(); ++x) {
                elt->set(x, y, computeRed_(elt, x, y), computeGreen_(elt, x, y),
                         computeBlue_(elt, x, y));
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

  private:
    ComputePixel computeRed_;
    ComputePixel computeGreen_;
    ComputePixel computeBlue_;
};

} // namespace hhimg

#endif
