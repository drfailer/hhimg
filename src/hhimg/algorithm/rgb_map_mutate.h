#ifndef RGB_MAP_MUTATE_HPP
#define RGB_MAP_MUTATE_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_tile_algorithm.h"
#include <functional>

namespace hhimg {

template <typename T>
struct RGBMapMutate : AbstractAlgorithm<T>, AbstractTileAlgorithm<T> {
    using ComputeImagePixel =
        std::function<T(Image<T>, size_t, size_t)>;
    using ComputeTilePixel = std::function<T(Tile<T>, size_t, size_t)>;

    // for images
    RGBMapMutate(ComputeImagePixel &&computeRed,
                 ComputeImagePixel &&computeGreen,
                 ComputeImagePixel &&computeBlue)
        : computeImageRed_(computeRed), computeImageGreen_(computeGreen),
          computeImageBlue_(computeBlue) {}

    // for tiles
    RGBMapMutate(ComputeTilePixel &&computeRed,
                 ComputeTilePixel &&computeGreen,
                 ComputeTilePixel &&computeBlue)
        : computeTileRed_(computeRed), computeTileGreen_(computeGreen),
          computeTileBlue_(computeBlue) {}

    Image<T> operator()(Image<T> image) const override {
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                Pixel<T> value = {
                    computeImageRed_(image, x, y),
                    computeImageGreen_(image, x, y),
                    computeImageBlue_(image, x, y),
                };
                image->set(x, y, value);
            }
        }
        return image;
    }

    void operator()(Tile<T> tile) override {
        for (size_t y = 0; y < tile->height(); ++y) {
            for (size_t x = 0; x < tile->width(); ++x) {
                Pixel<T> value = {
                    computeTileRed_(tile, x, y),
                    computeTileGreen_(tile, x, y),
                    computeTileBlue_(tile, x, y),
                };
                tile->set(x, y, value);
            }
        }
        this->addResult(tile);
    }

  private:
    struct ComputePixelDefault {
      T operator()(auto, size_t, size_t) {
        throw std::bad_function_call();
        return 0;
      }
    };

    // images
    ComputeImagePixel computeImageRed_ = ComputePixelDefault();
    ComputeImagePixel computeImageGreen_ = ComputePixelDefault();
    ComputeImagePixel computeImageBlue_ = ComputePixelDefault();

    // tiles
    ComputeTilePixel computeTileRed_ = ComputePixelDefault();
    ComputeTilePixel computeTileGreen_ = ComputePixelDefault();
    ComputeTilePixel computeTileBlue_ = ComputePixelDefault();
};

} // namespace hhimg

#endif
