#ifndef CONCEPTS_H
#define CONCEPTS_H
#include "../abstract/data/abstract_image.h"
#include "../abstract/data/abstract_tile.h"
#include "../abstract/hh/abstract_pair_tile_algorithm.h"
#include "../abstract/hh/abstract_tile_algorithm.h"
#include <type_traits>

namespace hhimg {

template <typename T> struct clear {
    using type = std::remove_const_t<std::remove_reference_t<T>>;
};

template <typename T> struct clear<std::shared_ptr<T>> {
    using type = typename clear<T>::type;
};

template <typename T> using clear_t = typename clear<T>::type;

template <typename Type>
concept ImgData =
    std::is_base_of_v<AbstractImage<typename clear_t<Type>::PixelType>,
                      clear_t<Type>> ||
    std::is_base_of_v<AbstractTile<typename clear_t<Type>::PixelType>,
                      clear_t<Type>>;

template <typename Type>
concept PairTileAlgorithm = std::is_base_of_v<
    AbstractPairTileAlgorithm<typename clear_t<Type>::PixelType>,
    clear_t<Type>>;

template <typename Type>
concept TileAlgorithm = std::is_base_of_v<
    AbstractTileAlgorithm<typename clear_t<Type>::PixelType>,
    clear_t<Type>>;

template <typename Type>
concept TileAlgorithms = TileAlgorithm<Type> || PairTileAlgorithm<Type>;

template <typename Type>
using pixel_type_t = typename clear_t<Type>::PixelType;

template <typename Type>
concept HHPipeline = requires(clear_t<Type> p) {
  p.ghostRegionSize();
  p.add(std::declval<std::shared_ptr<AbstractTileAlgorithm<uint8_t>>>());
};

} // end namespace hhimg

#endif
