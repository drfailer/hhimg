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

    ImgData<T> operator()(ImgData<T> image) const override {
        utils::PerfRectorder::start("Crop");
        auto output = imageFactory_->get(width_, height_);

        if (x_ + width_ >= image->width() || y_ + height_ >= image->height()) {
            throw std::invalid_argument("error: invalid crop dimentions");
        }

        // test width_ and height_ valid ?
        for (size_t y = 0; y < width_; ++y) {
            for (size_t x = 0; x < height_; ++x) {
                T red = image->red(x + x_, y + y_);
                T green = image->green(x + x_, y + y_);
                T blue = image->blue(x + x_, y + y_);
                output->set(x, y, red, green, blue);
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
