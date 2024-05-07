#ifndef CROP_HPP
#define CROP_HPP
#include "../abstract/abstract_algorithm.h"
#include "hhimg/abstract/abstract_image_factory.h"
#include <cstddef>
#include <memory>
#include <stdexcept>

namespace hhimg {

template <typename T> class Crop : public AbstractAlgorithm<T> {
  public:
    Crop(size_t x, size_t y, size_t width, size_t height,
         std::shared_ptr<AbstractImageFactory<T>> imageFactory)
        : x_(x), y_(y), width_(width), height_(height),
          imageFactory_(imageFactory) {}
    ~Crop() = default;

    ImgData<T> operator()(ImgData<T> image) const override {
        auto output = imageFactory_->get(width_, height_);

        if (x_ + width_ >= image->width() || y_ + height_ >= image->height()) {
            throw std::invalid_argument("error: invalid crop dimentions");
        }

        // test width_ and height_ valid ?
        for (size_t y = 0; y < width_; ++y) {
            for (size_t x = 0; x < height_; ++x) {
                output->at(x, y)->set(image->at(x + x_, y + y_));
            }
        }
        return output;
    }

  private:
    size_t x_ = 0;
    size_t y_ = 0;
    size_t width_ = 0;
    size_t height_ = 0;
    std::shared_ptr<AbstractImageFactory<T>> imageFactory_ = nullptr;
};

} // namespace hhimg

#endif
