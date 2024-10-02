#ifndef ABSTRACT_ALGORITHM_HPP
#define ABSTRACT_ALGORITHM_HPP
#include "../../common/abstract_image.h"

namespace hhimg::seq {

template <typename T> struct AbstractAlgorithm {
    AbstractAlgorithm() = default;
    virtual ~AbstractAlgorithm() {}

    virtual Image<T> operator()(Image<T> image) const = 0;
};

} // namespace hhimg::seq

#endif
