#ifndef MASK_APPLIER_HPP
#define MASK_APPLIER_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_image_factory.h"
#include "../concrete/data/mask.h"
#include "../tools/perf_recorder.h"
#include "../tools/utils.h"

namespace hhimg {

template <typename T, typename MaskType>
class Convolute : public AbstractAlgorithm<T> {
  public:
    Convolute(std::shared_ptr<AbstractImageFactory<T>> imageFactory,
              Mask<MaskType> const &kernel, MaskType bias = 0)
        : imageFactory_(imageFactory), kernel_(kernel), bias_(bias) {}
    ~Convolute() = default;

    ImgData<T> operator()(ImgData<T> image) const override {
        utils::PerfRectorder::start("Convolute");
        size_t halfWidth = kernel_.width() / 2;
        size_t halfHeight = kernel_.height() / 2;
        size_t beginX = halfWidth, endX = image->width() - halfWidth;
        size_t beginY = halfHeight, endY = image->height() - halfHeight;
        auto result = imageFactory_->get(image->width() - kernel_.width() + 1,
                                         image->height() - kernel_.height() + 1);

        for (size_t y = beginY; y < endY; ++y) {
            for (size_t x = beginX; x < endX; ++x) {
                auto value = computeValue(x, y, image);
                size_t rx = x - beginX;
                size_t ry = y - beginY;
                result->set(rx, ry, validate<T>(value + bias_));
            }
        }
        utils::PerfRectorder::end("Convolute");
        return result;
    }

  private:
    std::shared_ptr<AbstractImageFactory<T>> imageFactory_ = nullptr;
    Mask<MaskType> kernel_;
    MaskType bias_ = 0;

    Pixel<MaskType> computeValue(size_t x, size_t y, ImgData<T> image) const {
        int halfWidth = kernel_.width() / 2;
        int halfHeight = kernel_.height() / 2;
        Pixel<MaskType> result = {0, 0, 0};

        for (size_t my = 0; my < kernel_.height(); ++my) {
            for (size_t mx = 0; mx < kernel_.width(); ++mx) {
                size_t ix = x + mx - halfWidth;
                size_t iy = y + my - halfHeight;
                result += image->template at<MaskType>(ix, iy) * kernel_.at(mx, my);
            }
        }
        return result;
    }
};

} // namespace hhimg

#endif
