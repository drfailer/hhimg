#ifndef HHIMG_UPDATE_STENCILS_STATE_MANAGER_H
#define HHIMG_UPDATE_STENCILS_STATE_MANAGER_H
#include "update_stencils_state.h"
#include <hedgehog/hedgehog.h>

namespace hhimg::hdg {

template <typename T>
using UpdateStencilsStateManagerType =
    hh::StateManager<2, AbstractTile<T>, PairTile<T>, PairTile<T>,
                     AbstractTile<T>>;

template <typename T>
struct UpdateStencilsStateManager : UpdateStencilsStateManagerType<T> {
    UpdateStencilsStateManager(std::shared_ptr<UpdateStencilsState<T>> state)
        : UpdateStencilsStateManagerType<T>(state, "Stencil State") {}

    [[nodiscard]] bool canTerminate() const override {
        this->state()->lock();
        bool ret =
            std::dynamic_pointer_cast<UpdateStencilsState<T>>(this->state())
                ->isDone();
        this->state()->unlock();
        return ret;
    }
};

} // end namespace hhimg::hdg

#endif
