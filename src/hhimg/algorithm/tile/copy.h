#ifndef HHIMG_COPY_H
#define HHIMG_COPY_H
#include "../../abstract/abstract_tile_algorithm.h"

namespace hhimg {

template <typename T> struct Copy : AbstractTileAlgorithm<T> {
    Copy(size_t nbThreads) : AbstractTileAlgorithm<T>("Copy", nbThreads) {}

    void operator()(Tile<T> tile) override {
      for (size_t y = 0; y < tile->height(); ++y) {
        for (size_t x = 0; x < tile->width(); ++x) {
          size_t ix = tile->x() + x;
          size_t iy = tile->y() + y;
          tile->image()->set(ix, iy, tile->at(x, y));
        }
      }
      this->addResult(tile);
    }

    std::shared_ptr<TaskType<T>> copy() override {
        return std::make_shared<Copy>(this->numberThreads());
    }
};

} // end namespace hhimg

#endif
