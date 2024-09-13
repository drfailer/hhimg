#ifndef ABSTRACT_ALGORITHM_HPP
#define ABSTRACT_ALGORITHM_HPP
#include "../abstract/data/abstract_image.h"
#include <memory>

namespace hhimg {

template <typename T> class AbstractImage;
template <typename T> using Image = std::shared_ptr<AbstractImage<T>>;

template <typename T> struct AbstractAlgorithm {
    AbstractAlgorithm() = default;
    virtual ~AbstractAlgorithm() {}

    virtual Image<T> operator()(Image<T> image) const = 0;
};

} // namespace hhimg

#endif
