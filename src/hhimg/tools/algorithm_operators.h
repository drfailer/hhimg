#ifndef ALGORITHM_OPERATORS_HPP
#define ALGORITHM_OPERATORS_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_tile_algorithm.h"
#include "../abstract/abstract_hh_algorithm.h"
#include "../concrete/chain_algorithm.h"
#include "hhimg/algorithm/tile/split.h"
#include <memory>

/******************************************************************************/
/*                                   images                                   */
/******************************************************************************/

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

/******************************************************************************/
/*                                   tiles                                    */
/******************************************************************************/

template <typename Img, typename T>
std::shared_ptr<Img>
operator|=(std::shared_ptr<Img> image,
           std::shared_ptr<hhimg::AbstractHHAlgorithm<T>> algorithm) {
  return std::dynamic_pointer_cast<Img>(algorithm->operator()(image));
}

template <typename T>
std::shared_ptr<hhimg::AbstractHHAlgorithm<T>>
operator|(std::shared_ptr<hhimg::Split<T>> split,
          std::shared_ptr<hhimg::AbstractTileAlgorithm<T>> algorithm) {
    split->push_back(algorithm);
    return split;
}

template <typename T>
std::shared_ptr<hhimg::AbstractHHAlgorithm<T>>
operator|(std::shared_ptr<hhimg::AbstractHHAlgorithm<T>> hhAlgo,
          std::shared_ptr<hhimg::AbstractTileAlgorithm<T>> algorithm) {
    hhAlgo->push_back(algorithm);
    return hhAlgo;
}

template <typename T>
std::shared_ptr<hhimg::AbstractHHAlgorithm<T>>
operator|(std::shared_ptr<hhimg::AbstractTileAlgorithm<T>> algorithm,
          std::shared_ptr<hhimg::AbstractHHAlgorithm<T>> hhAlgo) {
    hhAlgo->push_front(algorithm);
    return hhAlgo;
}

#endif
