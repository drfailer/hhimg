#ifndef ABSTRACT_ALGORITHM_HPP
#define ABSTRACT_ALGORITHM_HPP

namespace hhimg {

template <typename T> class AbstractImage;

template <typename T> struct AbstractAlgorithm {
    AbstractAlgorithm() = default;
    virtual ~AbstractAlgorithm() {}

    virtual AbstractImage<T> &operator()(AbstractImage<T> &image) const = 0;
};

} // namespace hhimg

#endif
