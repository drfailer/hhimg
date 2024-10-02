#ifndef MINUS_WRAPPER_H
#define MINUS_WRAPPER_H
#include "../../../abstract/data/abstract_tile.h"
#include "hedgehog/src/api/task/abstract_task.h"

namespace hhimg::hdg {

template <typename T> struct MinusTile {
    MinusTile(std::shared_ptr<AbstractTile<T>> tile) : tile(tile) {}
    std::shared_ptr<AbstractTile<T>> tile = nullptr;
};

template <typename T>
struct MinusTileWrapper : hh::AbstractTask<1, AbstractTile<T>, MinusTile<T>> {
    MinusTileWrapper(size_t nbThreads)
        : hh::AbstractTask<1, AbstractTile<T>, MinusTile<T>>(
              "Minus Tile Wrapper", nbThreads) {}

    void execute(std::shared_ptr<AbstractTile<T>> tile) override {
        this->addResult(std::make_shared<MinusTile<T>>(tile));
    }

    std::shared_ptr<hh::AbstractTask<1, AbstractTile<T>, MinusTile<T>>>
    copy() override {
        return std::make_shared<MinusTileWrapper<T>>(this->numberThreads());
    }
};

} // end namespace hhimg::hdg

#endif
