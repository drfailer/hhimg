#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP
#include "../abstract/data/abstract_image.h"

namespace hhimg::concepts {

template <typename InnerType>
concept ValidType =
    std::is_base_of_v<AbstractImage<typename InnerType::type>, InnerType> ||
    std::is_base_of_v<AbstractPixel<typename InnerType::type>, InnerType>;

}

#endif
