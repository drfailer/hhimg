#ifndef SEQ_OPERATORS_H
#define SEQ_OPERATORS_H
#include "abstract/abstract_algorithm.h"
#include "concrete/chain_algorithm.h"

// TODO: if possible use a constraint to check if Img derive from AbstractImage
template <typename Img, typename T>
std::shared_ptr<Img>
operator|=(std::shared_ptr<Img> image,
           hhimg::seq::AbstractAlgorithm<T> const &algorithm) {
    image->set(algorithm(image));
    return image;
}

template <typename T>
hhimg::seq::ChainAlgorithm<T>
operator|(hhimg::seq::AbstractAlgorithm<T> const &algorithm1,
          hhimg::seq::AbstractAlgorithm<T> const &algorithm2) {
    return hhimg::seq::ChainAlgorithm<T>(algorithm1, algorithm2);
}

#endif
