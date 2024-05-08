#ifndef RGB_MAP_MUTATE_HPP
#define RGB_MAP_MUTATE_HPP
#include "../abstract/abstract_algorithm.h"
#include <functional>

namespace hhimg {

template <typename T> struct RGBMapMutate : public AbstractAlgorithm<T> {
    RGBMapMutate(std::function<T(size_t, size_t)> const &computeRed,
                 std::function<T(size_t, size_t)> const &computeGreen,
                 std::function<T(size_t, size_t)> const &computeBlue)
        : computeRed_(computeRed), computeGreen_(computeGreen),
          computeBlue_(computeBlue) {}

    ImgData<T> operator()(ImgData<T> image) const override {
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                Pixel<T> value = {
                    computeRed_(x, y),
                    computeGreen_(x, y),
                    computeBlue_(x, y),
                };
                image->set(x, y, value);
            }
        }
        return image;
    }

  private:
    std::function<T(size_t, size_t)> computeRed_;
    std::function<T(size_t, size_t)> computeGreen_;
    std::function<T(size_t, size_t)> computeBlue_;
};

} // namespace hhimg

#endif
