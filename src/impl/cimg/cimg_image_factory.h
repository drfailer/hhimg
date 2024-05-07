#ifndef CIMG_FACTORY_HPP
#define CIMG_FACTORY_HPP
#include "impl/cimg/cimg_image.h"
#include <hhimg/hhimg.h>

template <typename T>
class CImgImageFactory : public hhimg::AbstractImageFactory<T> {
    std::shared_ptr<hhimg::AbstractImage<T>> get(size_t width,
                                                 size_t height) const override {
        return std::make_shared<CImgImage<T>>(width, height);
    }
};

#endif
