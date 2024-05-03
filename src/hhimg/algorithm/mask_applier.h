#ifndef MASK_APPLIER_HPP
#define MASK_APPLIER_HPP
#include "../abstract/data/abstract_image.h"
#include "../concrete/data/mask.h"

namespace hhimg {

template <typename T> class MaskApplier {
  public:
    MaskApplier(Mask<T> const &mask) : mask_(mask) {}
    ~MaskApplier() = default;

    void operator()(std::shared_ptr<AbstractImage<T>> image) {
        size_t halfWidth = mask_.width() / 2;
        size_t halfHeight = mask_.height() / 2;
        size_t beginX = halfWidth, endX = image->width() - halfWidth;
        size_t beginY = halfHeight, endY = image->height() - halfHeight;

        for (size_t y = beginY; y < endY; ++y) {
            for (size_t x = beginX; x < endX; ++x) {
                applyMask(x, y, image);
            }
        }
    }

  private:
    Mask<T> mask_;

    void applyMask(size_t x, size_t y,
                   std::shared_ptr<AbstractImage<T>> image) {
        T result[3] = {0};
        int halfWidth = mask_.width() / 2;
        int halfHeight = mask_.height() / 2;

        for (int my = -halfHeight; my <= halfHeight; ++my) {
            for (int mx = -halfWidth; mx <= halfWidth; ++mx) {
                auto pixel = image->at(x + mx, y + my);
                int value = mask_.get(my + halfHeight, mx + halfWidth);
                result[0] += pixel.red() * value;
                result[1] += pixel.green() * value;
                result[2] += pixel.blue() * value;
            }
        }
        image->at(x, y)->set(validate(result[0]), validate(result[1]),
                             validate(result[2]));
    }

    int validate(int value) const {
        if (value < 0)
            return 0;
        if (value > 255)
            return 255;
        return value;
    }
};

} // namespace hhimg

#endif
