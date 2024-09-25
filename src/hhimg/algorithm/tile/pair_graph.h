#ifndef HHIMG_PAIR_GRAPH_H
#define HHIMG_PAIR_GRAPH_H
#include "../../abstract/hh/abstract_pair_tile_algorithm.h"
#include "../../algorithm/tile/create_tmp_tiles.h"
#include "ghost_region_state.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
struct PairGraph : hh::Graph<1, AbstractTile<T>, AbstractTile<T>> {
    PairGraph(std::shared_ptr<AbstractPairTileAlgorithm<T>> task)
        : hh::Graph<1, AbstractTile<T>, AbstractTile<T>>(task->name()) {
        auto makePairTask = std::make_shared<hhimg::CreateTmpTiles<T>>(1);
        auto ghostRegionState = std::make_shared<hhimg::SyncGhostRegions<T>>();
        auto ghostRegionsStateManager =
            std::make_shared<hh::StateManager<1, PairTile<T>, PairTile<T>>>(
                ghostRegionState);

        this->inputs(makePairTask);
        this->edges(makePairTask, ghostRegionsStateManager);
        this->edges(ghostRegionsStateManager, task);
        this->outputs(task);
    }
};

} // namespace hhimg

#endif
