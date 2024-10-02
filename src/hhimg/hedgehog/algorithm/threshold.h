#ifndef HDG_THRESHOLD_HPP
#define HDG_THRESHOLD_HPP
#include "../abstract/tile_algorithms.h"

namespace hhimg::hdg {

template <typename T> class Threshold : public AbstractTileAlgorithm<T> {
  public:
    Threshold(size_t nbThreads, T max, T lowValue, T highValue)
        : AbstractTileAlgorithm<T>(nbThreads, "Threshold"), max_(max),
          lowValue_(lowValue), highValue_(highValue) {}
    Threshold(T max, T lowValue, T highValue)
        : Threshold(1, max, lowValue, highValue) {}
    Threshold(T max) : Threshold(1, max, 0, 255) {}
    Threshold(size_t nbThreads, T max) : Threshold(nbThreads, max, 0, 255) {}

    void operator()(std::shared_ptr<AbstractTile<T>> tile) override {
        for (size_t y = 0; y < tile->height(); ++y) {
            for (size_t x = 0; x < tile->width(); ++x) {
                if (tile->get(x, y) < max_) {
                    tile->set(x, y, lowValue_);
                } else {
                    tile->set(x, y, highValue_);
                }
            }
        }
        this->addResult(tile);
    }

    std::shared_ptr<typename AbstractTileAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<Threshold<T>>(this->numberThreads(), max_,
                                              lowValue_, highValue_);
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }

  private:
    T max_ = 0;
    T lowValue_ = 0;
    T highValue_ = 0;
};

} // namespace hhimg::hdg

#endif
