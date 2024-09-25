#ifndef SPLIT_TASK_H
#define SPLIT_TASK_H
#include "../../../abstract/abstract_tile_factory.h"
#include "../../../abstract/data/abstract_tile.h"
#include "split_state.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
struct SplitTask : hh::AbstractTask<1, TileParams<T>, AbstractTile<T>> {
    SplitTask(size_t nbThreads, size_t tileSize, size_t ghostRegionSize,
              std::shared_ptr<AbstractTileFactory<T>> tileFactory)
        : hh::AbstractTask<1, TileParams<T>, AbstractTile<T>>("Split Task",
                                                              nbThreads),
          tileSize(tileSize), ghostRegionSize(ghostRegionSize),
          tileFactory(tileFactory) {}

    void execute(std::shared_ptr<TileParams<T>> params) {
        auto image = std::get<0>(*params);
        auto x = std::get<1>(*params);
        auto y = std::get<2>(*params);
        this->addResult(std::static_pointer_cast<AbstractTile<T>>(
            tileFactory->get(x, y, tileSize, ghostRegionSize, image)));
    }

    std::shared_ptr<hh::AbstractTask<1, TileParams<T>, AbstractTile<T>>>
    copy() override {
        return std::make_shared<SplitTask<T>>(this->numberThreads(), tileSize,
                                              ghostRegionSize, tileFactory);
    }

    size_t tileSize = 0;
    size_t ghostRegionSize = 0;
    std::shared_ptr<AbstractTileFactory<T>> tileFactory = nullptr;
};

} // end namespace hhimg

#endif
