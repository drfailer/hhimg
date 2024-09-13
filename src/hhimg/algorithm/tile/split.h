#ifndef SPLIT_H
#define SPLIT_H
#include "../../abstract/abstract_hh_algorithm.h"
#include "../../abstract/abstract_tile_factory.h"

namespace hhimg {

template <typename T> struct Split : AbstractHHAlgorithm<T> {
    // todo: ghost values
    Split(size_t tileSize, std::shared_ptr<AbstractTileFactory<T>> tileFactory)
        : tileFactory(tileFactory), tileSize(tileSize) {}
    ~Split() = default;

    Image<T> operator()(Image<T> image) override {
        // setup the factory with the image
        tileFactory->tileSize(tileSize);
        tileFactory->image(image);

        // get the number of tiles
        size_t nbTilesRows = tileFactory->nbTileRows();
        size_t nbTilesCols = tileFactory->nbTileColumns();

        this->compile();
        // todo: the graph is not reusable yet
        this->executeGraph();

        for (size_t i = 0; i < nbTilesRows; ++i) {
            for (size_t j = 0; j < nbTilesCols; ++j) {
                size_t x = j * tileSize;
                size_t y = i * tileSize;
                this->pushData(tileFactory->get(x, y));
            }
        }
        this->finishPushingData();
        this->waitForTermination();
        return image;
    }

    std::shared_ptr<AbstractTileFactory<T>> tileFactory = nullptr;
    size_t tileSize = 0;
};

} // end namespace hhimg

#endif
