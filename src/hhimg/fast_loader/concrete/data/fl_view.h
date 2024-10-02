#ifndef FL_VIEW_H
#define FL_VIEW_H
#include "fast_loader/api/view/default_view.h"
#include <fast_loader/fast_loader.h>

namespace hhimg::fld {

template <typename T>
using View = fl::DefaultView<T>;

} // end namespace hhimg::fld

#endif
