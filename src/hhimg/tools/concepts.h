#ifndef CONCEPTS_HPP
#define CONCEPTS_HPP
#include "../abstract/data/abstract_pixel.h"

namespace hhimg::concepts {

// This is for operators, for now operators should be used only with pixels.
// Arithmetic operations on images should be done with algorithms
template <typename InnerType>
concept ValidType =
    std::is_base_of_v<AbstractPixel<typename InnerType::type>, InnerType>;

}

#endif
