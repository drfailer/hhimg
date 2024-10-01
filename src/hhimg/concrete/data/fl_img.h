#ifndef FL_IMG_H
#define FL_IMG_H
#include <fast_loader/fast_loader.h>
#include "fl_view.h"
#include <memory>

namespace hhimg {

template <typename T> struct FLImg {
    std::shared_ptr<fl::AbstractTileLoader<View<T>>> tileLoader =
        nullptr;
};

} // end namespace hhimg

#endif
