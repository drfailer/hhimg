#ifndef MASK_APPLIER_HPP
#define MASK_APPLIER_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/data/abstract_image.h"
#include "../concrete/data/mask.h"
#include <iostream>

namespace hhimg {

template <typename T> class MaskApplier : public AbstractAlgorithm<T> {
  public:
    using mask_type = double;
    MaskApplier(Mask<mask_type> const &mask) : mask_(mask) {}
    ~MaskApplier() = default;

    AbstractImage<T> &operator()(AbstractImage<T> &image) const override {
        size_t halfWidth = mask_.width() / 2;
        size_t halfHeight = mask_.height() / 2;
        size_t beginX = halfWidth, endX = image.width() - halfWidth;
        size_t beginY = halfHeight, endY = image.height() - halfHeight;

        for (size_t y = beginY; y < endY; ++y) {
            for (size_t x = beginX; x < endX; ++x) {
                applyMask(x, y, image);
            }
        }
        return image;
    }

  private:
    Mask<mask_type> mask_;

    void applyMask(size_t x, size_t y, AbstractImage<T> &image) const {
        mask_type result[3] = {0};
        int halfWidth = mask_.width() / 2;
        int halfHeight = mask_.height() / 2;

        for (size_t my = 0; my < mask_.height(); ++my) {
            for (size_t mx = 0; mx < mask_.width(); ++mx) {
                /* std::cout << "halfWidth: " << halfWidth << std::endl; */
                /* std::cout << "halfHeight: " << halfHeight << std::endl; */
                auto pixel = image.at(x + mx - halfWidth, y + my - halfHeight);
                mask_type value = mask_.get(mx, my);
                result[0] += pixel->red() * value;
                result[1] += pixel->green() * value;
                result[2] += pixel->blue() * value;
            }
        }
        image.at(x, y)->set(validate(result[0]), validate(result[1]),
                            validate(result[2]));
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
