#ifndef GRAY_SCALE_HPP
#define GRAY_SCALE_HPP
#include "../abstract/abstract_algorithm.h"
#include "../tools/perf_recorder.h"

namespace hhimg {

template <typename T> struct GrayScale : public AbstractAlgorithm<T> {
    GrayScale() = default;
    ~GrayScale() = default;

    ImgData<T> operator()(ImgData<T> image) const override {
        utils::PerfRectorder::start("GrayScale");
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                auto pixel = image->at(x, y);
                auto mean = (pixel.red + pixel.green + pixel.blue) / 3.0;
                image->set(x, y, mean);
            }
        }
        utils::PerfRectorder::end("GrayScale");
        return image;
    }
};

}; // namespace hhimg

#endif
