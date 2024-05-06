#ifndef MASK_APPLIER_HPP
#define MASK_APPLIER_HPP
#include "../abstract/abstract_algorithm.h"
#include "../concrete/data/mask.h"
#include "../tools/utils.h"

namespace hhimg {

template <typename T, typename MaskType>
class MaskApplier : public AbstractAlgorithm<T> {
  public:
    MaskApplier(Mask<MaskType> const &mask) : mask_(mask) {}
    ~MaskApplier() = default;

    ImgData<T> operator()(ImgData<T> image) const override {
        size_t halfWidth = mask_.width() / 2;
        size_t halfHeight = mask_.height() / 2;
        size_t beginX = halfWidth, endX = image->width() - halfWidth;
        size_t beginY = halfHeight, endY = image->height() - halfHeight;
        auto result = image->copy();

        for (size_t y = beginY; y < endY; ++y) {
            for (size_t x = beginX; x < endX; ++x) {
                auto value = computeValue(x, y, image);
                result->at(x, y)->set(utils::validate(value.red),
                                      utils::validate(value.green),
                                      utils::validate(value.blue));
            }
        }
        return result;
    }

  private:
    Mask<MaskType> mask_;

    struct PixelValue {
        MaskType red;
        MaskType green;
        MaskType blue;
    };

    PixelValue computeValue(size_t x, size_t y, ImgData<T> image) const {
        int halfWidth = mask_.width() / 2;
        int halfHeight = mask_.height() / 2;
        PixelValue result = {0, 0, 0};

        for (size_t my = 0; my < mask_.height(); ++my) {
            for (size_t mx = 0; mx < mask_.width(); ++mx) {
                auto pixel = image->at(x + mx - halfWidth, y + my - halfHeight);
                result.red += pixel->red() * mask_.get(mx, my, 0);
                result.green += pixel->green() * mask_.get(mx, my, 1);
                result.blue += pixel->blue() * mask_.get(mx, my, 2);
            }
        }
        return result;
    }
};

} // namespace hhimg

#endif
