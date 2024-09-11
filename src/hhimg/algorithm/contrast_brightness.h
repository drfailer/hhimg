#ifndef CONTRAST_BRIGHTNESS_H
#define CONTRAST_BRIGHTNESS_H
#include "../abstract/abstract_algorithm.h"
#include "../tools/perf_recorder.h"

namespace hhimg {

template <typename T> struct ContrastBrightness : public AbstractAlgorithm<T> {
    ContrastBrightness(double contrast, double brightness = 1)
        : contrast(contrast), brightness(brightness) {}
    ~ContrastBrightness() = default;

    T computeContrast(T value) const {
      double result = contrast * (value - 128.0) + 128.0 + brightness;

      if (result < 0) {
        return 0;
      } else if (result > 255) {
        return 255;
      }
      return T(result);
    }

    ImgData<T> operator()(ImgData<T> image) const override {
        utils::PerfRectorder::start("ContrastBrightness");
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                auto pixel = image->at(x, y);
                image->set(x, y, computeContrast(pixel.red),
                           computeContrast(pixel.green),
                           computeContrast(pixel.blue));
            }
        }
        utils::PerfRectorder::end("ContrastBrightness");
        return image;
    }

    double contrast = 1;
    double brightness = 0;
};

}; // namespace hhimg

#endif
