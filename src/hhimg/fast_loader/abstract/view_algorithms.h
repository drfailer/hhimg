#ifndef VIEW_ALGORITHMS_H
#define VIEW_ALGORITHMS_H
#include "../concrete/data/fl_view.h"
#include "abstract_fl_algorithm.h"
#include <hedgehog/hedgehog.h>

namespace hhimg::fld {

template <typename T>
using AbstractViewAlgorithm = AbstractFLAlgorithm<View<T>, View<T>>;

} // end namespace hhimg::fld

#endif
