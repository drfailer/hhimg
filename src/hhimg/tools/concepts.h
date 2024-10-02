#ifndef CONCEPTS_H
#define CONCEPTS_H
#include "../fast_loader/abstract/abstract_fl_algorithm.h"
#include "../fast_loader/concrete/fast_loader_pipeline.h"
#include "../hedgehog/abstract/abstract_hh_algorithm.h"
#include "../hedgehog/abstract/data/abstract_tile.h"
#include "../hedgehog/concrete/hedgehog_pipeline.h"
#include <memory>
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
struct hh_pipeline<hdg::HedgehogPipeline<T, F, L>> : std::true_type {};

template <typename T>
constexpr bool hh_pipeline_v = hh_pipeline<clear_t<T>>::value;

template <typename T> struct fl_pipeline : std::false_type {};

template <typename T, typename F, typename L>
struct fl_pipeline<fld::FastLoaderPipeline<T, F, L>> : std::true_type {};

template <typename T>
constexpr bool fl_pipeline_v = fl_pipeline<clear_t<T>>::value;

template <typename Type>
concept ImgData =
    std::is_base_of_v<AbstractImage<typename clear_t<Type>::PixelType>,
                      clear_t<Type>> ||
    std::is_base_of_v<hdg::AbstractTile<typename clear_t<Type>::PixelType>,
                      clear_t<Type>>;

template <typename Type>
concept TileAlgorithms = requires {
    typename clear_t<Type>::InputType;
    typename clear_t<Type>::OutputType;
    typename clear_t<Type>::TaskType;
    std::is_base_of_v<
        hdg::AbstractHHAlgorithm<typename clear_t<Type>::InputType,
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
    std::is_base_of_v<
        fld::AbstractFLAlgorithm<typename clear_t<Type>::InputType,
                                 typename clear_t<Type>::OutputType>,
        clear_t<Type>>;
};

template <typename Type>
concept FLPipeline = fl_pipeline_v<Type>;

} // end namespace hhimg

#endif
