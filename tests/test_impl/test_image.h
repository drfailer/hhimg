#ifndef TEST_IMAGE_HPP
#define TEST_IMAGE_HPP
#include "rgb_value.h"
#include <cstring>
#include <hhimg/hhimg.h>
#include <memory>

template <typename T> class TestImage : public hhimg::AbstractImage<T> {
  public:
    TestImage(RGBValue<T> *image, size_t width, size_t height)
        : hhimg::AbstractImage<T>(""), image_(image), width_(width),
          height_(height) {}
    TestImage(TestImage<T> const &other)
        : hhimg::AbstractImage<T>(other.filename()) {
        memcpy(image_, other.image_, width_ * height_ * sizeof(T));
    }

    size_t width() const override { return width_; }
    size_t height() const override { return height_; }

    void load(std::string const &) override {}
    void save(std::string const &) override {}

    std::shared_ptr<hhimg::AbstractImage<T>> copy() const override {
        RGBValue<unsigned char> *mem = new RGBValue<T>[width_ * height_];
        memcpy(mem, image_, width_ * height_ * sizeof(*mem));
        return std::make_shared<TestImage<T>>(mem, width_, height_);
    }

    void set(std::shared_ptr<hhimg::AbstractImage<T>> &&other) override {
        TestImage<T> *otherI = dynamic_cast<TestImage<T> *>(other.get());
        this->width_ = otherI->width_;
        this->height_ = otherI->height_;
        std::swap(this->image_, otherI->image_);
    }

    hhimg::Pixel<T> at(size_t offset) const override {
        RGBValue<T> value = image_[offset];
        return {value.red, value.green, value.blue};
    }

    void set(size_t offset, hhimg::Pixel<T> const &pixel) override {
        image_[offset].red = pixel.red;
        image_[offset].green = pixel.green;
        image_[offset].blue = pixel.blue;
    }

    using hhimg::AbstractImage<T>::at;
    using hhimg::AbstractImage<T>::set;

    friend bool operator==(std::shared_ptr<TestImage<T>> lhs,
                           std::shared_ptr<TestImage<T>> rhs) {
        for (size_t i = 0; i < lhs->width() * lhs->height(); ++i) {
            if (lhs.get()->image_[i] != rhs.get()->image_[i]) {
                return false;
            }
        }
        return true;
    }

  private:
    RGBValue<T> *image_ = {0};
    size_t width_ = 0;
    size_t height_ = 0;
};

#endif
