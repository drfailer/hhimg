#ifndef CONTRAST_BRIGHTNESS_H
#define CONTRAST_BRIGHTNESS_H
#include "../abstract/abstract_algorithm.h"
#include "../abstract/hh/abstract_tile_algorithm.h"
#include "../tools/concepts.h"
#include "../tools/perf_recorder.h"

namespace hhimg {

template <typename T>
struct ContrastBrightness : AbstractAlgorithm<T>, AbstractTileAlgorithm<T> {
    ContrastBrightness(size_t nbThreads, double contrast, double brightness)
        : AbstractTileAlgorithm<T>(nbThreads, "ContrastBrightness"),
          contrast(contrast), brightness(brightness) {}
    ContrastBrightness(double contrast, double brightness)
        : ContrastBrightness(1, contrast, brightness) {}
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

    void compute(ImgData auto elt) const {
        for (size_t y = 0; y < elt->height(); ++y) {
            for (size_t x = 0; x < elt->width(); ++x) {
                auto pixel = elt->at(x, y);
                elt->set(x, y, computeContrast(pixel.red),
                         computeContrast(pixel.green),
                         computeContrast(pixel.blue));
            }
        }
    }

    Image<T> operator()(Image<T> image) const override {
        utils::PerfRectorder::start("ContrastBrightness");
        compute(image);
        utils::PerfRectorder::end("ContrastBrightness");
        return image;
    }

    void operator()(std::shared_ptr<AbstractTile<T>> tile) override {
        compute(tile);
        this->addResult(tile);
    }

    std::shared_ptr<typename AbstractTileAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<ContrastBrightness<T>>(this->numberThreads(),
                                                       contrast, brightness);
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }

    double contrast = 1;
    double brightness = 0;
};

}; // namespace hhimg

#endif
