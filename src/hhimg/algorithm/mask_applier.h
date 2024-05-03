#ifndef MASK_APPLIER_HPP
#define MASK_APPLIER_HPP
#include "../abstract/abstract_algorithm.h"
#include "../concrete/data/mask.h"
#include <queue>

namespace hhimg {

template <typename T> class MaskApplier : public AbstractAlgorithm<T> {
  public:
    using mask_type = double;
    MaskApplier(Mask<mask_type> const &mask) : mask_(mask) {}
    ~MaskApplier() = default;

    ImgData<T> operator()(ImgData<T> image) const override {
        size_t halfWidth = mask_.width() / 2;
        size_t halfHeight = mask_.height() / 2;
        size_t beginX = halfWidth, endX = image->width() - halfWidth;
        size_t beginY = halfHeight, endY = image->height() - halfHeight;
        std::queue<PixelValue> values;

        for (size_t y = beginY; y < endY; ++y) {
            for (size_t x = beginX; x < endX; ++x) {
                values.push(computeValue(x, y, image));
            }
        }
        applyValues(image, values);
        return image;
    }

  private:
    Mask<mask_type> mask_;

    struct PixelValue {
        mask_type red;
        mask_type green;
        mask_type blue;
    };

    PixelValue computeValue(size_t x, size_t y, ImgData<T> image) const {
        int halfWidth = mask_.width() / 2;
        int halfHeight = mask_.height() / 2;
        PixelValue result = {0, 0, 0};

        for (size_t my = 0; my < mask_.height(); ++my) {
            for (size_t mx = 0; mx < mask_.width(); ++mx) {
                auto pixel = image->at(x + mx - halfWidth, y + my - halfHeight);
                mask_type value = mask_.get(mx, my);
                result.red += pixel->red() * value;
                result.green += pixel->green() * value;
                result.blue += pixel->blue() * value;
            }
        }
        return result;
    }

    void applyValues(ImgData<T> image, std::queue<PixelValue>& values) const {
        size_t halfWidth = mask_.width() / 2;
        size_t halfHeight = mask_.height() / 2;
        size_t beginX = halfWidth, endX = image->width() - halfWidth;
        size_t beginY = halfHeight, endY = image->height() - halfHeight;

        for (size_t y = beginY; y < endY; ++y) {
            for (size_t x = beginX; x < endX; ++x) {
                PixelValue value = values.front();
                image->at(x, y)->set(validate(value.red), validate(value.green),
                                    validate(value.blue));
                values.pop();
            }
        }
    }

    mask_type validate(mask_type value) const {
        if (value < 0)
            return 0;
        if (value > 255)
            return 255;
        return value;
    }
};

} // namespace hhimg

#endif
