#ifndef CONCEPTS_H
#define CONCEPTS_H
#include "../abstract/data/abstract_image.h"
#include "../abstract/data/abstract_tile.h"
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

} // end namespace hhimg

#endif
