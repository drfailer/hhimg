#ifndef HDG_CONTRAST_BRIGHTNESS_H
#define HDG_CONTRAST_BRIGHTNESS_H
#include "../abstract/tile_algorithms.h"

namespace hhimg::hdg {

template <typename T>
struct ContrastBrightness : AbstractTileAlgorithm<T> {
    ContrastBrightness(size_t nbThreads, double contrast, double brightness)
        : AbstractTileAlgorithm<T>(nbThreads, "ContrastBrightness"),
          contrast(contrast), brightness(brightness) {}
    ContrastBrightness(double contrast, double brightness)
        : ContrastBrightness(1, contrast, brightness) {}
    ~ContrastBrightness() = default;

    // TODO: should be common
    T computeContrast(T value) const {
        double result = contrast * (value - 128.0) + 128.0 + brightness;

        if (result < 0) {
            return 0;
        } else if (result > 255) {
            return 255;
        }
        return T(result);
    }

    void operator()(std::shared_ptr<AbstractTile<T>> tile) override {
        for (size_t y = 0; y < tile->height(); ++y) {
            for (size_t x = 0; x < tile->width(); ++x) {
                auto pixel = tile->at(x, y);
                tile->set(x, y, computeContrast(pixel.red),
                         computeContrast(pixel.green),
                         computeContrast(pixel.blue));
            }
        }
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

}; // namespace hhimg::hdg

#endif
