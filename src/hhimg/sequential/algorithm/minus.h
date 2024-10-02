#ifndef SEQ_MINUS_HPP
#define SEQ_MINUS_HPP
#include "../../tools/perf_recorder.h"
#include "../abstract/abstract_algorithm.h"

namespace hhimg::seq {

template <typename T> class Minus : public AbstractAlgorithm<T> {
  public:
    Minus(Image<T> const &imageToSubstract)
        : imageToSubstract_(imageToSubstract) {}

    Image<T> operator()(Image<T> image) const override {
        utils::PerfRectorder::start("Minus");
        if (image->width() != imageToSubstract_->width() ||
            image->height() != imageToSubstract_->height()) {
            throw std::invalid_argument(
                "error: can't substract images with different shapes");
        }
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                Pixel<T> p1 = image->at(x, y);
                Pixel<T> p2 = imageToSubstract_->at(x, y);
                image->set(x, y, p1 - p2);
            }
        }
        utils::PerfRectorder::end("Minus");
        return image;
    }

  private:
    Image<T> imageToSubstract_ = nullptr;
};

}; // namespace hhimg::seq

#endif
