#ifndef UPDATE_STENCILS_GRAPH_H
#define UPDATE_STENCILS_GRAPH_H
#include "../../../abstract/data/abstract_tile.h"
#include "update_stencils_state.h"
#include "update_stencils_state_manager.h"
#include "update_stencils_task.h"
#include <hedgehog/hedgehog.h>

namespace hhimg::hdg {

template <typename T>
struct UpdateStencilsGraph : hh::Graph<1, AbstractTile<T>, AbstractTile<T>> {
    UpdateStencilsGraph()
        : hh::Graph<1, AbstractTile<T>, AbstractTile<T>>("Update Stencils") {
        auto updateStencilsState = std::make_shared<UpdateStencilsState<T>>();
        auto updateStencilsStateManager =
            std::make_shared<UpdateStencilsStateManager<T>>(
                updateStencilsState);
        auto updateStencilsTask = std::make_shared<UpdateStencilsTask<T>>(1);

        this->inputs(updateStencilsStateManager);
        this->edges(updateStencilsStateManager, updateStencilsTask);
        this->edges(updateStencilsTask, updateStencilsStateManager);
        this->outputs(updateStencilsStateManager);
    }
};

} // namespace hhimg::hdg

#endif
