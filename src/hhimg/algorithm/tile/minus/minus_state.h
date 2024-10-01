#ifndef MINUS_STATE_H
#define MINUS_STATE_H
#include "../../../abstract/hh/tile_algorithms.h"
#include "minus_wrapper.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
struct MinusState
    : hh::AbstractState<2, MinusTile<T>, AbstractTile<T>, PairTile<T>> {

    void init(std::shared_ptr<AbstractTile<T>> tile) {
        nbTileRows_ = tile->image()->height() / tile->tileSize() +
                      (tile->image()->height() % tile->tileSize() == 0 ? 0 : 1);
        nbTileCols_ = tile->image()->width() / tile->tileSize() +
                      (tile->image()->width() % tile->tileSize() == 0 ? 0 : 1);
        tileCount_ = nbTileRows_ * nbTileCols_;
        tiles_.resize(tileCount_, {nullptr, nullptr});
    }

    size_t tileIdx(std::shared_ptr<AbstractTile<T>> tile) const {
        size_t tx = tile->x() / tile->tileSize();
        size_t ty = tile->y() / tile->tileSize();
        return ty * nbTileCols_ + tx;
    }

    void execute(std::shared_ptr<MinusTile<T>> minusTile) override {
        if (tiles_.empty()) {
            init(minusTile->tile);
        }
        size_t idx = tileIdx(minusTile->tile);

        tiles_[idx].second = minusTile->tile;
        if (tiles_[idx].first) {
            this->addResult(std::make_shared<PairTile<T>>(tiles_[idx]));
            --tileCount_;
        }
    }

    void execute(std::shared_ptr<AbstractTile<T>> tile) override {
        if (tiles_.empty()) {
            init(tile);
        }
        size_t idx = tileIdx(tile);

        tiles_[idx].first = tile;
        if (tiles_[idx].second) {
            this->addResult(std::make_shared<PairTile<T>>(tiles_[idx]));
            --tileCount_;
        }
    }

    bool isDone() const { return !tiles_.empty() && tileCount_ == 0; }

  private:
    size_t nbTileRows_ = 0, nbTileCols_ = 0, tileCount_ = 0;
    std::vector<PairTile<T>> tiles_ = {};
};

} // end namespace hhimg

#endif
