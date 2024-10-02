#ifndef HHIMG_SPLIT_GRAPH_H
#define HHIMG_SPLIT_GRAPH_H
#include "../../../../common/abstract_image.h"
#include "../../../abstract/data/abstract_tile.h"
#include "split_state.h"
#include "split_task.h"
#include <hedgehog/hedgehog.h>

namespace hhimg::hdg {

template <typename T>
struct SplitGraph : hh::Graph<1, AbstractImage<T>, AbstractTile<T>> {
    SplitGraph(size_t nbThreads, size_t tileSize, size_t ghostRegionSize,
               std::shared_ptr<AbstractTileFactory<T>> tileFactory)
        : hh::Graph<1, AbstractImage<T>, AbstractTile<T>>("Split") {
        auto splitTask = std::make_shared<SplitTask<T>>(
            nbThreads, tileSize, ghostRegionSize, tileFactory);
        auto splitState = std::make_shared<SplitState<T>>(tileSize);
        auto splitStateManager = std::make_shared<
            hh::StateManager<1, AbstractImage<T>, TileParams<T>>>(splitState);

        this->inputs(splitStateManager);
        this->edges(splitStateManager, splitTask);
        this->outputs(splitTask);
    }
};

} // end namespace hhimg::hdg

#endif
