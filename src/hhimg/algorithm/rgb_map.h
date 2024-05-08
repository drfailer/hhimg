#ifndef RGB_MAP_HPP
#define RGB_MAP_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_image_factory.h"
#include <functional>

namespace hhimg {

template <typename T> struct RGBMap : public AbstractAlgorithm<T> {
    RGBMap(
        std::shared_ptr<AbstractImageFactory<T>> imageFactory,
        std::function<T(ImgData<T> const &, size_t, size_t)> const &computeRed,
        std::function<T(ImgData<T> const &, size_t, size_t)> const
            &computeGreen,
        std::function<T(ImgData<T> const &, size_t, size_t)> const &computeBlue)
        : imageFactory_(imageFactory), computeRed_(computeRed),
          computeGreen_(computeGreen), computeBlue_(computeBlue) {}

    ImgData<T> operator()(ImgData<T> image) const override {
        auto result = imageFactory_->get(image->width(), image->height());

        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                Pixel<T> value = {
                    computeRed_(image, x, y),
                    computeGreen_(image, x, y),
                    computeBlue_(image, x, y),
                };
                result->set(x, y, value);
            }
        }
        return result;
    }

  private:
    std::shared_ptr<AbstractImageFactory<T>> imageFactory_ = nullptr;
    std::function<T(ImgData<T> const &, size_t, size_t)> computeRed_;
    std::function<T(ImgData<T> const &, size_t, size_t)> computeGreen_;
    std::function<T(ImgData<T> const &, size_t, size_t)> computeBlue_;
};

} // namespace hhimg

#endif
