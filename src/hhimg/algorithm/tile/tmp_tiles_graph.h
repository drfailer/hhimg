#ifndef HHIMG_TMP_TILES_GRAPH_H
#define HHIMG_TMP_TILES_GRAPH_H
#include "../../abstract/hh/tile_algorithms.h"
#include "../../algorithm/tile/create_tmp_tiles.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
struct TmpTilesGraph : hh::Graph<1, AbstractTile<T>, AbstractTile<T>> {
    TmpTilesGraph(std::shared_ptr<AbstractTmpTileAlgorithm<T>> task)
        : hh::Graph<1, AbstractTile<T>, AbstractTile<T>>(task->name()) {
        auto makePairTask = std::make_shared<hhimg::CreateTmpTiles<T>>(1);

        this->inputs(makePairTask);
        this->edges(makePairTask, task);
        this->outputs(task);
    }
};

} // namespace hhimg

#endif
