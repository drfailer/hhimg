#ifndef ABSTRACT_PIXEL_FACTORY_HPP
#define ABSTRACT_PIXEL_FACTORY_HPP
#include "data/abstract_pixel.h"
#include <memory>

namespace hhimg {

template <typename T> class AbstractPixelFactory {
  public:
    std::shared_ptr<AbstractPixel<T>> operator()() const = 0;

    std::shared_ptr<AbstractPixel<T>> operator()(T red, T green, T blue,
                                                 T alpha = 255) const {
        std::shared_ptr<AbstractPixel<T>> pixel = this->operator()();
        pixel->red(red);
        pixel->green(green);
        pixel->blue(blue);
        pixel->alpha(alpha);
        return pixel;
    }

    std::shared_ptr<AbstractPixel<T>> operator()(T value) const {
        return this->operator()(value, value, value);
    }
};

} // namespace hhimg

#endif
