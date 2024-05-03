#ifndef ABSTRACT_ALGORITHM_HPP
#define ABSTRACT_ALGORITHM_HPP
#include "../abstract/data/abstract_image.h"
#include <memory>
#include <iostream>

namespace hhimg {

template <typename T> class AbstractImage;
template <typename T> using ImgData = std::shared_ptr<AbstractImage<T>>;
/* template <typename T> using ImgData = AbstractImage<T>; */

template <typename T> struct AbstractAlgorithm {
    AbstractAlgorithm() = default;
    virtual ~AbstractAlgorithm() {}

    virtual ImgData<T> operator()(ImgData<T> image) const = 0;
};

// TODO: if possible use a constraint to check if Img derive from AbstractImage
template <typename Img, typename T>
std::shared_ptr<Img> operator|=(std::shared_ptr<Img> image, AbstractAlgorithm<T> const &algorithm) {
    auto result = algorithm(image);

    if (result.get() != image.get()) {
        for (size_t y = 0; y < result->height(); ++y) {
            for (size_t x = 0; x < result->width(); ++x) {
                image->at(x, y)->set(result->at(x, y));
            }
        }
    }

    return image;
}

} // namespace hhimg

#endif
