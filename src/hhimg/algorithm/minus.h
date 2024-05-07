#ifndef MINUS_HPP
#define MINUS_HPP
#include "../abstract/abstract_algorithm.h"
#include "../tools/perf_recorder.h"
#include "../tools/pixel_operators.h"
#include "../tools/shared_ptr_operators.h"

namespace hhimg {

template <typename T> class Minus : public AbstractAlgorithm<T> {
  public:
    Minus(ImgData<T> const &imageToSubstract)
        : imageToSubstract_(imageToSubstract) {}
    ~Minus() = default;

    ImgData<T> operator()(ImgData<T> image) const override {
        utils::PerfRectorder::start("Minus");
        if (image->width() != imageToSubstract_->width() ||
            image->height() != imageToSubstract_->height()) {
            throw std::invalid_argument(
                "error: can't substract images with different shapes");
        }
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                image->at(x, y) -= imageToSubstract_->at(x, y);
            }
        }
        utils::PerfRectorder::end("Minus");
        return image;
    }

  private:
    ImgData<T> imageToSubstract_ = nullptr;
};

}; // namespace hhimg

#endif
