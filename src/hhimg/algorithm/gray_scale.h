#ifndef GRAY_SCALE_HPP
#define GRAY_SCALE_HPP
#include "../tools/perf_recorder.h"
#include "hhimg/abstract/abstract_algorithm.h"

namespace hhimg {

template <typename T> struct GrayScale : public AbstractAlgorithm<T> {
    GrayScale() = default;
    ~GrayScale() = default;

    ImgData<T> operator()(ImgData<T> image) const override {
        utils::PerfRectorder::start("GrayScale");
        for (size_t i = 0; i < image->height(); ++i) {
            for (size_t j = 0; j < image->width(); ++j) {
                auto mean = (image->red(j, i) + image->green(j, i) +
                             image->blue(j, i)) /
                            3;
                image->set(j, i, mean);
            }
        }
        utils::PerfRectorder::end("GrayScale");
        return image;
    }
};

}; // namespace hhimg

#endif
