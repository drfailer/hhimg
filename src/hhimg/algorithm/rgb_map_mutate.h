#ifndef RGB_MAP_MUTATE_HPP
#define RGB_MAP_MUTATE_HPP
#include "../abstract/abstract_algorithm.h"
#include <functional>

namespace hhimg {

template <typename T> struct RGBMapMutate : public AbstractAlgorithm<T> {
    using ComputeValueFn = std::function<T(ImgData<T> const &, size_t, size_t)>;
    RGBMapMutate(ComputeValueFn const &computeRed,
                 ComputeValueFn const &computeGreen,
                 ComputeValueFn const &computeBlue)
        : computeRed_(computeRed), computeGreen_(computeGreen),
          computeBlue_(computeBlue) {}

    ImgData<T> operator()(ImgData<T> image) const override {
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                Pixel<T> value = {
                    computeRed_(image, x, y),
                    computeGreen_(image, x, y),
                    computeBlue_(image, x, y),
                };
                image->set(x, y, value);
            }
        }
        return image;
    }

  private:
    ComputeValueFn computeRed_;
    ComputeValueFn computeGreen_;
    ComputeValueFn computeBlue_;
};

} // namespace hhimg

#endif
