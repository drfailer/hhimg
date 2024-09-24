#ifndef HHIMG_PAIR_GRAPH_H
#define HHIMG_PAIR_GRAPH_H
#include "../../abstract/hh/abstract_pair_tile_algorithm.h"
#include "../../algorithm/tile/make_pair.h"
#include "../../algorithm/tile/stencil_state.h"
#include "../../algorithm/tile/stencil_state_manager.h"
#include "ghost_region_state.h"
#include "hhimg/algorithm/tile/update_stencil_task.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
struct PairGraph : hh::Graph<1, AbstractTile<T>, AbstractTile<T>> {
    PairGraph(std::shared_ptr<AbstractPairTileAlgorithm<T>> task)
        : hh::Graph<1, AbstractTile<T>, AbstractTile<T>>(task->name()) {
        auto makePairTask = std::make_shared<hhimg::MakePair<T>>(1);
        auto ghostRegionState = std::make_shared<hhimg::SyncGhostRegions<T>>();
        auto ghostRegionsStateManager =
            std::make_shared<hh::StateManager<1, PairTile<T>, PairTile<T>>>(
                ghostRegionState);
        auto stencilState = std::make_shared<StencilState<T>>();
        auto stencilStateManager =
            std::make_shared<StencilStateManager<T>>(stencilState);
        auto updateStencilTask = std::make_shared<UpdateStencilsTask<T>>(1);

        this->inputs(makePairTask);
        this->edges(makePairTask, ghostRegionsStateManager);
        this->edges(ghostRegionsStateManager, task);
        this->edges(task, stencilStateManager);
        this->edges(stencilStateManager, updateStencilTask);
        this->edges(updateStencilTask, stencilStateManager);
        this->outputs(stencilStateManager);
    }
};

} // namespace hhimg

#endif
