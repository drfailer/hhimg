#ifndef HHIMG_NULL_TYPE_H
#define HHIMG_NULL_TYPE_H
#include <type_traits>

namespace hhimg {

struct NullType {};

template <typename T> struct null_type : std::false_type {};

template <> struct null_type<NullType> : std::true_type {};

template <typename Type> constexpr bool null_type_v = null_type<Type>::value;

} // end namespace hhimg

#endif
