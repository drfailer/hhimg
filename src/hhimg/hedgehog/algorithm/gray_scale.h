#ifndef HDG_GRAY_SCALE_HPP
#define HDG_GRAY_SCALE_HPP
#include "../abstract/tile_algorithms.h"

namespace hhimg::hdg {

template <typename T>
struct GrayScale : AbstractTileAlgorithm<T> {
    GrayScale(size_t nbThreads = 1)
        : AbstractTileAlgorithm<T>(nbThreads, "GrayScale") {}
    ~GrayScale() = default;

    void operator()(std::shared_ptr<AbstractTile<T>> tile) override {
        for (size_t y = 0; y < tile->height(); ++y) {
            for (size_t x = 0; x < tile->width(); ++x) {
                auto pixel = tile->at(x, y);
                auto mean = (pixel.red + pixel.green + pixel.blue) / 3.0;
                tile->set(x, y, mean);
            }
        }
        this->addResult(tile);
    }

    std::shared_ptr<typename AbstractTileAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<GrayScale<T>>(this->numberThreads());
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }
};

}; // namespace hhimg::hdg

#endif
