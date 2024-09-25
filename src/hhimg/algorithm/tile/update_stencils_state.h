#ifndef HHIMG_UPDATE_STENCILS_STATE_H
#define HHIMG_UPDATE_STENCILS_STATE_H
#include "../../abstract/data/abstract_tile.h"
#include "../../abstract/hh/abstract_pair_tile_algorithm.h"
#include <hedgehog/hedgehog.h>
#include <vector>

namespace hhimg {

template <typename T>
using UpdateStencilsStateType =
    hh::AbstractState<2, AbstractTile<T>, PairTile<T>, PairTile<T>,
                      AbstractTile<T>>;

template <typename T>
class UpdateStencilsState : public UpdateStencilsStateType<T> {
  public:
    UpdateStencilsState() : UpdateStencilsStateType<T>() {}

    using TileType = std::pair<size_t, std::shared_ptr<AbstractTile<T>>>;

    void initTiles(std::shared_ptr<AbstractTile<T>> tile) {
        nbTileRows_ = tile->image()->height() / tile->tileSize() +
                      (tile->image()->height() % tile->tileSize() == 0 ? 0 : 1);
        nbTileCols_ = tile->image()->width() / tile->tileSize() +
                      (tile->image()->width() % tile->tileSize() == 0 ? 0 : 1);
        tiles_.resize(nbTileRows_ * nbTileCols_, {0, nullptr});
        tileCount_ = nbTileRows_ * nbTileCols_;
    }

    TileType &pairsAt(size_t x, size_t y) {
        return tiles_[y * nbTileCols_ + x];
    }

    void execute(std::shared_ptr<AbstractTile<T>> tile) override {
        size_t x = tile->x() / tile->tileSize();
        size_t y = tile->y() / tile->tileSize();

        if (tiles_.empty()) {
            initTiles(tile);
        }
        pairsAt(x, y).second = tile;
        pairsAt(x, y).first = 8;

        for (auto [modx, mody] : mods) {
            size_t x2 = x + modx;
            size_t y2 = y + mody;
            if (x2 < nbTileCols_ && y2 < nbTileRows_) {
                if (pairsAt(x2, y2).second) {
                    auto otherTile = pairsAt(x2, y2).second;
                    this->addResult(
                        std::make_shared<PairTile<T>>(tile, otherTile));
                }
            } else {
                pairsAt(x, y).first--;
            }
        }
    }

    void execute(std::shared_ptr<PairTile<T>> tiles) {
        registerUpdate(tiles->first);
        registerUpdate(tiles->second);
    }

    void registerUpdate(std::shared_ptr<AbstractTile<T>> tile) {
        size_t x = tile->x() / tile->tileSize();
        size_t y = tile->y() / tile->tileSize();

        pairsAt(x, y).first--;
        if (pairsAt(x, y).first <= 0) {
            this->addResult(tile);
            --tileCount_;
        }
    }

    bool isDone() const { return !tiles_.empty() && tileCount_ == 0; }

  private:
    size_t nbTileRows_ = 0;
    size_t nbTileCols_ = 0;
    size_t tileCount_ = 0;
    std::vector<TileType> tiles_ = {};
    static constexpr std::pair<int, int> mods[8]{
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1},
    };
};

} // end namespace hhimg

#endif
