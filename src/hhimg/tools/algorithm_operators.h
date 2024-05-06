#ifndef ALGORITHM_OPERATORS_HPP
#define ALGORITHM_OPERATORS_HPP
#include "../abstract/abstract_algorithm.h"
#include "../concrete/chain_algorithm.h"
#include <memory>

// TODO: if possible use a constraint to check if Img derive from AbstractImage
template <typename Img, typename T>
std::shared_ptr<Img> operator|=(std::shared_ptr<Img> image,
                                hhimg::AbstractAlgorithm<T> const &algorithm) {
    image->set(algorithm(image));
    return image;
}

template <typename T>
hhimg::ChainAlgorithm<T>
operator|(hhimg::AbstractAlgorithm<T> const &algorithm1,
          hhimg::AbstractAlgorithm<T> const &algorithm2) {
    return hhimg::ChainAlgorithm<T>(algorithm1, algorithm2);
}

#endif
