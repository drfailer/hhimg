#ifndef HHIMG_PAIR_GRAPH_H
#define HHIMG_PAIR_GRAPH_H
#include "../../abstract/abstract_hh_algorithm.h"
#include "../../abstract/abstract_pair_tile_algorithm.h"
#include "../../algorithm/tile/make_pair.h"
#include "./ghost_region_state.h"

namespace hhimg {

template <typename T> struct PairGraph : GraphType<T> {
    PairGraph(std::shared_ptr<AbstractPairTileAlgorithm<T>> task)
        : GraphType<T>("Convolution") {
        auto makePairTask = std::make_shared<hhimg::MakePair<T>>(1);
        auto ghostRegionState = std::make_shared<hhimg::SyncGhostRegions<T>>();
        auto ghostRegionsStateManager = std::make_shared<
            hh::StateManager<1, std::pair<hhimg::Tile<T>, hhimg::Tile<T>>,
                             std::pair<hhimg::Tile<T>, hhimg::Tile<T>>>>(
            ghostRegionState);

        this->inputs(makePairTask);
        this->edges(makePairTask, ghostRegionsStateManager);
        this->edges(ghostRegionsStateManager, task);
        this->outputs(task);
    }
};

} // namespace hhimg

#endif
