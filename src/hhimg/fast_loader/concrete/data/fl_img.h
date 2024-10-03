#ifndef FL_IMG_H
#define FL_IMG_H
#include <fast_loader/fast_loader.h>
#include "fl_view.h"
#include <memory>

namespace hhimg::fld {

template <typename T, typename TileWriter> struct FLImg {
    std::shared_ptr<fl::AbstractTileLoader<View<T>>> tileLoader =
        nullptr;
    std::shared_ptr<TileWriter> tileWriter = nullptr;
};

} // end namespace hhimg::fld

#endif
