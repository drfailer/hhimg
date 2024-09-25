#ifndef UPDATE_STENCILS_GRAPH_H
#define UPDATE_STENCILS_GRAPH_H
#include <hedgehog/hedgehog.h>
#include "../../abstract/data/abstract_tile.h"
#include "stencil_state.h"
#include "stencil_state_manager.h"
#include "update_stencil_task.h"

namespace hhimg {

template <typename T>
struct UpdateStencilsGraph : hh::Graph<1, AbstractTile<T>, AbstractTile<T>> {
  UpdateStencilsGraph()
    : hh::Graph<1, AbstractTile<T>, AbstractTile<T>>("Update Stencils") {
        auto stencilState = std::make_shared<StencilState<T>>();
        auto stencilStateManager =
            std::make_shared<StencilStateManager<T>>(stencilState);
        auto updateStencilTask = std::make_shared<UpdateStencilsTask<T>>(1);

        this->inputs(stencilStateManager);
        this->edges(stencilStateManager, updateStencilTask);
        this->edges(updateStencilTask, stencilStateManager);
        this->outputs(stencilStateManager);
    }
};

}

#endif
