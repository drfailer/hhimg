#ifndef NON_MAXIMUM_SUPPRESSION_HPP
#define NON_MAXIMUM_SUPPRESSION_HPP
#include "hhimg/abstract/abstract_algorithm.h"

namespace hhimg {

template <typename T>
class NonMaximumSuppression : public AbstractAlgorithm<T> {
  public:
    NonMaximumSuppression(T max, T lowValue_ = 0, T highValue = 255)
        : max_(max), lowValue_(lowValue_), highValue_(highValue) {}

    ImgData<T> operator()(ImgData<T> image) const override {
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                if (image->at(x, y)->get() < max_) {
                    image->at(x, y)->set(lowValue_);
                } else {
                    image->at(x, y)->set(highValue_);
                }
            }
        }
        return image;
    }

  private:
    T max_ = 0;
    T lowValue_ = 0;
    T highValue_ = 0;
};

} // namespace hhimg

#endif
