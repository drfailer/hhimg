#ifndef TEST_IMAGE_HPP
#define TEST_IMAGE_HPP
#include "rgb_value.h"
#include "test_pixel.h"
#include <cstring>
#include <hhimg/hhimg.h>
#include <memory>

template <typename T> class TestImage : public hhimg::AbstractImage<T> {
  public:
    TestImage(RGBValue<T> *image, size_t width, size_t height)
        : hhimg::AbstractImage<T>(""), image_(image), width_(width),
          height_(height) {}
    TestImage(TestImage<T> const &other) {
        memcpy(image_, other.image_, width_ * height_ * sizeof(T));
    }
    ~TestImage() { delete image_; }

    size_t width() const override { return width_; }
    size_t height() const override { return height_; }

    void load(std::string const &) override {}
    void save(std::string const &) override {}

    friend bool operator==(std::shared_ptr<TestImage<T>> lhs,
                           std::shared_ptr<TestImage<T>> rhs) {
        for (size_t i = 0; i < lhs->width() * lhs->height(); ++i) {
            if (lhs.get()->image_[i] != rhs.get()->image_[i]) {
                return false;
            }
        }
        return true;
    }

    std::shared_ptr<hhimg::AbstractImage<T>> copy() const override {
        return std::make_shared<TestImage<T>>(new RGBValue<T>[width_ * height_],
                                              width_, height_);
    }

    void set(std::shared_ptr<hhimg::AbstractImage<T>> &&other) override {
        TestImage<T> *otherI = dynamic_cast<TestImage<T> *>(other.get());
        this->width_ = otherI->width_;
        this->height_ = otherI->height_;
        std::swap(this->image_, otherI->image_);
    }

  private:
    RGBValue<T> *image_ = {0};
    size_t width_ = 0;
    size_t height_ = 0;

    std::shared_ptr<hhimg::AbstractPixel<T>> atImpl(size_t offset) override {
        auto pixel = std::make_shared<TestPixel<T>>(
            image_[offset].red, image_[offset].green, image_[offset].blue);
        return pixel;
    }

    std::shared_ptr<hhimg::AbstractPixel<T>> const
    atImpl(size_t offset) const override {
        auto pixel = std::make_shared<hhimg::PixelValue<T>>(
            image_[offset].red, image_[offset].green, image_[offset].blue);
        return pixel;
    }
};

#endif
