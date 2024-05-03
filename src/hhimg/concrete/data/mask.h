#ifndef MASK_HPP
#define MASK_HPP
#include <cstddef>
#include <stdexcept>
#include <vector>

namespace hhimg {

template <typename T> class Mask {
  private:
    Mask(size_t width, size_t height) : width_(width), height_(height) {
        if (width % 2 == 0 || height % 2 == 0) {
            throw std::invalid_argument("error: the width and the height of a "
                                        "mask must be an odd value.");
        }
    }

  public:
    Mask(std::vector<T> mask, size_t width, size_t height)
        : Mask(width, height) {
        for (size_t i = 0; i < height * width; ++i) {
            mask_.push_back(std::vector<T>(3, mask[i]));
        }
    }

    Mask(std::vector<std::vector<T>> mask, size_t width, size_t height)
        : Mask(width, height) {
        for (size_t i = 0; i < height * width; ++i) {
            mask_.emplace_back(mask[i]);
        }
    }

    virtual ~Mask() {}

    size_t width() const { return width_; }
    size_t height() const { return height_; }

    T get(size_t mx, size_t my) const { return mask_[my * width_ + mx][0]; }
    T get(size_t mx, size_t my, size_t colorIdx) const {
        return mask_[my * width_ + mx][colorIdx];
    }

  private:
    std::vector<std::vector<T>> mask_ = {};
    size_t width_ = 0;
    size_t height_ = 0;
};

} // namespace hhimg

#endif
