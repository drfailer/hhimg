#ifndef TEST_IMAGE_HPP
#define TEST_IMAGE_HPP
#include "test_pixel.h"
#include <cstring>
#include <hhimg/hhimg.h>
#include <memory>
#define TEST_IMAGE_WIDTH 10
#define TEST_IMAGE_HEIGHT 10

template <typename T, size_t Width, size_t Height>
class TestImage : public hhimg::AbstractImage<T> {
  public:
    TestImage() = default;
    TestImage(TestImage<T, Width, Height> const &other) {
        memcpy(image_, other.image_, Width * Height * sizeof(T));
    }

    size_t width() const override { return Width; }
    size_t height() const override { return Height; }
    size_t size() const override { return Width * Height; }

    void load(std::string const &) override {}
    void save(std::string const &) override {}

    struct Pixel {
        T red;
        T green;
        T blue;
    };

    friend bool operator==(Pixel const &lhs, Pixel const &rhs) {
        return lhs.red == rhs.red && lhs.green == rhs.green &&
               lhs.blue == rhs.blue;
    }

    friend bool operator==(std::shared_ptr<TestImage<T, Width, Height>> lhs,
                           std::shared_ptr<TestImage<T, Width, Height>> rhs) {
        for (size_t i = 0; i < Width * Height; ++i) {
            if (lhs.get()->image_[i] != rhs.get()->image_[i]) {
                return false;
            }
        }
        return true;
    }

  private:
    Pixel image_[Width * Height] = {0};

    std::shared_ptr<hhimg::AbstractPixel<T>> atImpl(size_t offset) override {
        auto pixel = std::make_shared<TestPixel<T>>(
            image_[offset].red, image_[offset].green, image_[offset].blue);
        return pixel;
    }
};

#endif
