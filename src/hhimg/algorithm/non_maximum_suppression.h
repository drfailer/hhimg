#ifndef NON_MAXIMUM_SUPPRESSION_HPP
#define NON_MAXIMUM_SUPPRESSION_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/hh/tile_algorithms.h"
#include "../tools/concepts.h"
#include "../tools/perf_recorder.h"

namespace hhimg {

template <typename T>
class NonMaximumSuppression : public AbstractAlgorithm<T>,
                              public AbstractTileAlgorithm<T> {
  public:
    NonMaximumSuppression(size_t nbThreads, T max, T lowValue, T highValue)
        : AbstractTileAlgorithm<T>(nbThreads, "NonMaximumSuppression"),
          max_(max), lowValue_(lowValue), highValue_(highValue) {}
    NonMaximumSuppression(T max, T lowValue, T highValue)
        : NonMaximumSuppression(1, max, lowValue, highValue) {}
    NonMaximumSuppression(T max) : NonMaximumSuppression(1, max, 0, 255) {}
    NonMaximumSuppression(size_t nbThreads, T max)
        : NonMaximumSuppression(nbThreads, max, 0, 255) {}

    void compute(ImgData auto &elt) const {
        for (size_t y = 0; y < elt->height(); ++y) {
            for (size_t x = 0; x < elt->width(); ++x) {
                if (elt->get(x, y) < max_) {
                    elt->set(x, y, lowValue_);
                } else {
                    elt->set(x, y, highValue_);
                }
            }
        }
    }

    Image<T> operator()(Image<T> image) const override {
        utils::PerfRectorder::start("NonMaximumSuppression");
        compute(image);
        utils::PerfRectorder::end("NonMaximumSuppression");
        return image;
    }

    void operator()(std::shared_ptr<AbstractTile<T>> tile) override {
        compute(tile);
        this->addResult(tile);
    }

    std::shared_ptr<typename AbstractTileAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<NonMaximumSuppression<T>>(
            this->numberThreads(), max_, lowValue_, highValue_);
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }

  private:
    T max_ = 0;
    T lowValue_ = 0;
    T highValue_ = 0;
};

} // namespace hhimg

#endif
