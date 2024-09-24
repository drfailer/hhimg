#ifndef STENCIL_STATE_MANAGER_H
#define STENCIL_STATE_MANAGER_H
#include "stencil_state.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
using StencilStateManagerType =
    hh::StateManager<2, AbstractTile<T>, PairTile<T>, PairTile<T>,
                     AbstractTile<T>>;

template <typename T> struct StencilStateManager : StencilStateManagerType<T> {
    StencilStateManager(std::shared_ptr<StencilState<T>> state)
        : StencilStateManagerType<T>(state, "Stencil State") {}

    [[nodiscard]] bool canTerminate() const override {
        this->state()->lock();
        bool ret =
            std::dynamic_pointer_cast<StencilState<T>>(this->state())->isDone();
        this->state()->unlock();
        return ret;
    }
};

} // end namespace hhimg

#endif
