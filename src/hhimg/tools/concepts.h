#ifndef CONCEPTS_H
#define CONCEPTS_H
#include "../abstract/data/abstract_image.h"
#include "../abstract/data/abstract_tile.h"
#include "../abstract/fast_loader/abstract_fl_algorithm.h"
#include "../abstract/hh/tile_algorithms.h"
#include "../concrete/fast_loader_pipeline.h"
#include "../concrete/hedgehog_pipeline.h"
#include <type_traits>

namespace hhimg {

template <typename T> struct clear {
    using type = std::remove_const_t<std::remove_reference_t<T>>;
};

template <typename T> struct clear<std::shared_ptr<T>> {
    using type = typename clear<T>::type;
};

template <typename T> using clear_t = typename clear<T>::type;

template <typename T> struct hh_pipeline : std::false_type {};

template <typename T, typename F, typename L>
struct hh_pipeline<HedgehogPipeline<T, F, L>> : std::true_type {};

template <typename T>
constexpr bool hh_pipeline_v = hh_pipeline<clear_t<T>>::value;

template <typename T> struct fl_pipeline : std::false_type {};

template <typename T, typename F, typename L>
struct fl_pipeline<FastLoaderPipeline<T, F, L>> : std::true_type {};

template <typename T>
constexpr bool fl_pipeline_v = fl_pipeline<clear_t<T>>::value;

template <typename Type>
concept ImgData =
    std::is_base_of_v<AbstractImage<typename clear_t<Type>::PixelType>,
                      clear_t<Type>> ||
    std::is_base_of_v<AbstractTile<typename clear_t<Type>::PixelType>,
                      clear_t<Type>>;

template <typename Type>
concept TileAlgorithms = requires {
    typename clear_t<Type>::InputType;
    typename clear_t<Type>::OutputType;
    typename clear_t<Type>::TaskType;
    std::is_base_of_v<AbstractHHAlgorithm<typename clear_t<Type>::InputType,
                                          typename clear_t<Type>::OutputType>,
                      clear_t<Type>>;
};

template <typename Type> using pixel_type_t = typename clear_t<Type>::PixelType;

template <typename Type>
concept HHPipeline = hh_pipeline_v<Type>;

template <typename Type>
concept FLAlgorithms = requires {
    typename clear_t<Type>::InputType;
    typename clear_t<Type>::OutputType;
    typename clear_t<Type>::TaskType;
    std::is_base_of_v<AbstractFLAlgorithm<typename clear_t<Type>::InputType,
                                          typename clear_t<Type>::OutputType>,
                      clear_t<Type>>;
};

template <typename Type>
concept FLPipeline = fl_pipeline_v<Type>;

} // end namespace hhimg

#endif
