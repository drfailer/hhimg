#ifndef MINUS_STATE_MANAGER_H
#define MINUS_STATE_MANAGER_H
#include "../../../abstract/tile_algorithms.h"
#include "minus_state.h"
#include "minus_wrapper.h"
#include <hedgehog/hedgehog.h>

namespace hhimg::hdg {

template <typename T>
struct MinusStateManager
    : hh::StateManager<2, MinusTile<T>, AbstractTile<T>, PairTile<T>> {
    MinusStateManager(
        std::shared_ptr<
            hh::AbstractState<2, MinusTile<T>, AbstractTile<T>, PairTile<T>>>
            state)
        : hh::StateManager<2, MinusTile<T>, AbstractTile<T>, PairTile<T>>(
              state, "Minus State") {}

    [[nodiscard]] bool canTerminate() const override {
        this->state()->lock();
        bool ret =
            std::dynamic_pointer_cast<MinusState<T>>(this->state())->isDone();
        this->state()->unlock();
        return ret;
    }
};

} // end namespace hhimg::hdg

#endif
