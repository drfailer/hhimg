#ifndef SPLIT_H
#define SPLIT_H
#include "../../abstract/abstract_tile_factory.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
struct Split : hh::AbstractTask<1, AbstractImage<T>, AbstractTile<T>> {
    // todo: ghost values
    Split(size_t tileSize, std::shared_ptr<AbstractTileFactory<T>> tileFactory)
        : hh::AbstractTask<1, AbstractImage<T>, AbstractTile<T>>("Split"),
          tileSize(tileSize), tileFactory(tileFactory) {}
    ~Split() = default;

    void execute(std::shared_ptr<AbstractImage<T>> image) override {
        size_t nbTilesRows = tileFactory->nbTileRows(image->height(), tileSize);
        size_t nbTilesCols =
            tileFactory->nbTileColumns(image->width(), tileSize);

        for (size_t i = 0; i < nbTilesRows; ++i) {
            for (size_t j = 0; j < nbTilesCols; ++j) {
                size_t x = j * tileSize;
                size_t y = i * tileSize;
                auto tile =
                    tileFactory->get(x, y, tileSize, ghostRegionSize, image);
                this->addResult(
                    std::static_pointer_cast<AbstractTile<T>>(tile));
            }
        }
    }

    size_t tileSize = 0;
    size_t ghostRegionSize = 0;
    std::shared_ptr<AbstractTileFactory<T>> tileFactory = nullptr;
};

} // end namespace hhimg

#endif
