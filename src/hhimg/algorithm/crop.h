#ifndef CROP_HPP
#define CROP_HPP
#include "../abstract/abstract_algorithm.h"
#include "../tools/perf_recorder.h"
#include "hhimg/abstract/abstract_image_factory.h"
#include <cstddef>
#include <memory>
#include <stdexcept>

namespace hhimg {

template <typename T> class Crop : public AbstractAlgorithm<T> {
  public:
    Crop(std::shared_ptr<AbstractImageFactory<T>> imageFactory, size_t x,
         size_t y, size_t width, size_t height)
        : x_(x), y_(y), width_(width), height_(height),
          imageFactory_(imageFactory) {}
    ~Crop() = default;

    Image<T> operator()(Image<T> image) const override {
        utils::PerfRectorder::start("Crop");
        auto output = imageFactory_->get(width_, height_);

        if (x_ + width_ >= image->width() || y_ + height_ >= image->height()) {
            throw std::invalid_argument("error: invalid crop dimentions");
        }

        for (size_t y = 0; y < height_; ++y) {
            for (size_t x = 0; x < width_; ++x) {
                output->set(x, y, image->at(x + x_, y + y_));
            }
        }
        utils::PerfRectorder::end("Crop");
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
