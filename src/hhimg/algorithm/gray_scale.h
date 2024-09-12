#ifndef GRAY_SCALE_HPP
#define GRAY_SCALE_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_tile_algorithm.h"
#include "../tools/perf_recorder.h"

namespace hhimg {

template <typename T>
struct GrayScale : AbstractAlgorithm<T>, AbstractTileAlgorithm<T> {
    GrayScale() = default;
    ~GrayScale() = default;

    void grayScale(auto &image) const {
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                auto pixel = image->at(x, y);
                auto mean = (pixel.red + pixel.green + pixel.blue) / 3.0;
                image->set(x, y, mean);
            }
        }
    }

    ImgData<T> operator()(ImgData<T> image) const override {
        utils::PerfRectorder::start("GrayScale");
        grayScale(image);
        utils::PerfRectorder::end("GrayScale");
        return image;
    }

    void operator()(Tile<T> tile,
                    std::function<void(Tile<T>)> addResult) const override {
        grayScale(tile);
        addResult(tile);
    }
};

}; // namespace hhimg

#endif
