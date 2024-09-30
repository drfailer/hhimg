#ifndef FL_IMG_H
#define FL_IMG_H
#include <memory>

namespace hhimg {

template <typename TileLoader>
struct FLImg {
  std::shared_ptr<TileLoader> tileLoader = nullptr;
};

} // end namespace hhimg

#endif
