#ifndef NON_MAXIMUM_SUPPRESSION_HPP
#define NON_MAXIMUM_SUPPRESSION_HPP
#include "../tools/perf_recorder.h"
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_tile_algorithm.h"

namespace hhimg {

template <typename T>
class NonMaximumSuppression : public AbstractAlgorithm<T>,
                              public AbstractTileAlgorithm<T> {
  public:
    NonMaximumSuppression(T max, T lowValue_ = 0, T highValue = 255)
        : max_(max), lowValue_(lowValue_), highValue_(highValue) {}

    void compute(auto &elt) const {
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

    ImgData<T> operator()(ImgData<T> image) const override {
        utils::PerfRectorder::start("NonMaximumSuppression");
        compute(image);
        utils::PerfRectorder::end("NonMaximumSuppression");
        return image;
    }

    void operator()(Tile<T> tile, std::function<void(Tile<T>)> addResult) const override {
      compute(tile);
      addResult(tile);
    }

  private:
    T max_ = 0;
    T lowValue_ = 0;
    T highValue_ = 0;
};

} // namespace hhimg

#endif
