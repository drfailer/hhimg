#ifndef HHIMG_SPLIT_STATE_H
#define HHIMG_SPLIT_STATE_H
#include "../../../../common/abstract_image.h"
#include <hedgehog/hedgehog.h>
#include <tuple>

namespace hhimg::hdg {

template <typename T>
using TileParams = std::tuple<std::shared_ptr<AbstractImage<T>>, size_t, size_t>;

template <typename T>
struct SplitState : hh::AbstractState<1, AbstractImage<T>, TileParams<T>> {
    SplitState(size_t tileSize)
        : hh::AbstractState<1, AbstractImage<T>, TileParams<T>>(),
          tileSize_(tileSize) {}

    size_t nbTileRows(size_t imageHeight, size_t tileSize) const {
        return imageHeight / tileSize + (imageHeight % tileSize == 0 ? 0 : 1);
    }

    size_t nbTileColumns(size_t imageWidth, size_t tileSize) const {
        return imageWidth / tileSize + (imageWidth % tileSize == 0 ? 0 : 1);
    }

    void execute(std::shared_ptr<AbstractImage<T>> image) override {
        size_t nbTilesRows = nbTileRows(image->height(), tileSize_);
        size_t nbTilesCols = nbTileColumns(image->width(), tileSize_);

        for (size_t i = 0; i < nbTilesRows; ++i) {
            for (size_t j = 0; j < nbTilesCols; ++j) {
                size_t x = j * tileSize_;
                size_t y = i * tileSize_;
                this->addResult(std::make_shared<TileParams<T>>(image, x, y));
            }
        }
    }

  private:
    size_t tileSize_ = 0;
};

} // end namespace hhimg::hdg

#endif
