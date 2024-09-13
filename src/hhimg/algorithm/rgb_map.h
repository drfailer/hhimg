#ifndef RGB_MAP_HPP
#define RGB_MAP_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_image_factory.h"
#include <functional>

namespace hhimg {

template <typename T> struct RGBMap : public AbstractAlgorithm<T> {
    using ComputeValueFn = std::function<T(Image<T> const &, size_t, size_t)>;
    RGBMap(std::shared_ptr<AbstractImageFactory<T>> imageFactory,
           ComputeValueFn const &computeRed, ComputeValueFn const &computeGreen,
           ComputeValueFn const &computeBlue)
        : imageFactory_(imageFactory), computeRed_(computeRed),
          computeGreen_(computeGreen), computeBlue_(computeBlue) {}

    Image<T> operator()(Image<T> image) const override {
        auto result = imageFactory_->get(image->width(), image->height());

        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                result->set(x, y, computeRed_(image, x, y),
                            computeGreen_(image, x, y),
                            computeBlue_(image, x, y));
            }
        }
        return result;
    }

  private:
    std::shared_ptr<AbstractImageFactory<T>> imageFactory_ = nullptr;
    ComputeValueFn computeRed_;
    ComputeValueFn computeGreen_;
    ComputeValueFn computeBlue_;
};

} // namespace hhimg

#endif
