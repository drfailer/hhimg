#ifndef HHIMG_WIRE_H
#define HHIMG_WIRE_H
#include "../../abstract/abstract_tile_algorithm.h"

namespace hhimg {

template <typename T> struct Wire : AbstractTileAlgorithm<T> {
    Wire(size_t nbThreads) : AbstractTileAlgorithm<T>("Wire", nbThreads) {}
    void operator()(Tile<T> tile) override { this->addResult(tile); }

    std::shared_ptr<TaskType<T>> copy() override {
        return std::make_shared<Wire>(this->numberThreads());
    }
};

} // end namespace hhimg

#endif
