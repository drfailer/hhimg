#ifndef SEQ_GRAY_SCALE_HPP
#define SEQ_GRAY_SCALE_HPP
#include "../../tools/perf_recorder.h"
#include "../abstract/abstract_algorithm.h"

namespace hhimg::seq {

template <typename T> struct GrayScale : AbstractAlgorithm<T> {
    Image<T> operator()(Image<T> image) const override {
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

}; // namespace hhimg::seq

#endif
