#ifndef MASK_HPP
#define MASK_HPP
#include "pixel.h"
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
    Mask(std::vector<T> kernel, size_t width, size_t height)
        : Mask(width, height) {
        for (size_t i = 0; i < height * width; ++i) {
            kernel_.push_back(Pixel<T>{kernel[i], kernel[i], kernel[i]});
        }
    }

    Mask(std::vector<Pixel<T>> kernel, size_t width, size_t height)
        : Mask(width, height) {
        kernel_ = std::move(kernel);
    }

    virtual ~Mask() {}

    size_t width() const { return width_; }
    size_t height() const { return height_; }

    T get(size_t mx, size_t my, size_t colorIdx = 0) const {
        switch (colorIdx) {
        case 0:
            return kernel_[my * width_ + mx].red;
        case 1:
            return kernel_[my * width_ + mx].green;
        case 2:
            return kernel_[my * width_ + mx].blue;
        }
    }

    T red(size_t mx, size_t my) const { return kernel_[my * width_ + mx].red; }
    T green(size_t mx, size_t my) const {
        return kernel_[my * width_ + mx].green;
    }
    T blue(size_t mx, size_t my) const {
        return kernel_[my * width_ + mx].blue;
    }

    Pixel<T> at(size_t mx, size_t my) const {
        return kernel_[my * width_ + mx];
    }

  private:
    std::vector<Pixel<T>> kernel_ = {};
    size_t width_ = 0;
    size_t height_ = 0;
};

} // namespace hhimg

#endif
