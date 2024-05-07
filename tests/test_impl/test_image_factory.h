#ifndef TEST_IMAGE_FACTORY_HPP
#define TEST_IMAGE_FACTORY_HPP
#include "test_image.h"
#include <hhimg/hhimg.h>

template <typename T>
class TestImageFactory : public hhimg::AbstractImageFactory<T> {
  public:
    TestImageFactory() = default;
    ~TestImageFactory() = default;

    std::shared_ptr<hhimg::AbstractImage<T>> get(size_t width,
                                                 size_t height) const override {
        RGBValue<T> *mem = new RGBValue<T>[width * height];
        return std::make_shared<TestImage<T>>(mem, width, height);
    }
};

#endif
