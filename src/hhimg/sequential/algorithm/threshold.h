#ifndef SEQ_THRESHOLD_HPP
#define SEQ_THRESHOLD_HPP
#include "../../tools/perf_recorder.h"
#include "../abstract/abstract_algorithm.h"

namespace hhimg::seq {

template <typename T> class Threshold : public AbstractAlgorithm<T> {
  public:
    Threshold(T max, T lowValue = 0, T highValue = 255)
        : max_(max), lowValue_(lowValue), highValue_(highValue) {}

    Image<T> operator()(Image<T> image) const override {
        utils::PerfRectorder::start("NonMaximumSuppression");
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                if (image->get(x, y) < max_) {
                    image->set(x, y, lowValue_);
                } else {
                    image->set(x, y, highValue_);
                }
            }
        }
        utils::PerfRectorder::end("NonMaximumSuppression");
        return image;
    }

  private:
    T max_ = 0;
    T lowValue_ = 0;
    T highValue_ = 0;
};

} // namespace hhimg::seq

#endif
