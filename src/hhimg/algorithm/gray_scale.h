#ifndef GRAY_SCALE_HPP
#define GRAY_SCALE_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_tile_algorithm.h"
#include "../tools/perf_recorder.h"
#include "../tools/concepts.h"

namespace hhimg {

template <typename T>
struct GrayScale : AbstractAlgorithm<T>, AbstractTileAlgorithm<T> {
    GrayScale(size_t nbThreads = 1):
      AbstractTileAlgorithm<T>("GrayScale", nbThreads) {}
    ~GrayScale() = default;

    void compute(ImgData auto &image) const {
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                auto pixel = image->at(x, y);
                auto mean = (pixel.red + pixel.green + pixel.blue) / 3.0;
                image->set(x, y, mean);
            }
        }
    }

    Image<T> operator()(Image<T> image) const override {
        utils::PerfRectorder::start("GrayScale");
        compute(image);
        utils::PerfRectorder::end("GrayScale");
        return image;
    }

    void operator()(Tile<T> tile) override {
        compute(tile);
        this->addResult(tile);
    }

    std::shared_ptr<TaskType<T>> copy() override {
      return std::make_shared<GrayScale<T>>(this->numberThreads());
    }
};

}; // namespace hhimg

#endif
