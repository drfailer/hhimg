#ifndef GRAY_SCALE_HPP
#define GRAY_SCALE_HPP
#include "hhimg/abstract/abstract_algorithm.h"

namespace hhimg {

template <typename T> struct GrayScale : public AbstractAlgorithm<T> {
    GrayScale() = default;
    ~GrayScale() = default;

    ImgData<T> operator()(ImgData<T> image) const override {
        for (size_t i = 0; i < image->height(); ++i) {
            for (size_t j = 0; j < image->width(); ++j) {
                auto p = image->at(j, i);
                auto mean = (p->red() + p->green() + p->blue()) / 3;
                p->set(mean);
            }
        }
        return image;
    }
};

}; // namespace hhimg

#endif