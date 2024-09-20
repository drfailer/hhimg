#ifndef GHOST_REGION_STATE_H
#define GHOST_REGION_STATE_H
#include "../../abstract/hh/abstract_pair_tile_algorithm.h"
#include "../../abstract/hh/abstract_tile_algorithm.h"
#include <hedgehog/hedgehog.h>
#include <vector>

namespace hhimg {

template <typename T>
using GhostRegionState = hh::AbstractState<1, PairTile<T>, PairTile<T>>;

template <typename T> class SyncGhostRegions : public GhostRegionState<T> {
  public:
    SyncGhostRegions() : GhostRegionState<T>() {}

    void initPairs(std::shared_ptr<AbstractTile<T>> tile) {
        nbTileRows_ = tile->image()->height() / tile->tileSize() +
                      (tile->image()->height() % tile->tileSize() == 0 ? 0 : 1);
        nbTileCols_ = tile->image()->width() / tile->tileSize() +
                      (tile->image()->width() % tile->tileSize() == 0 ? 0 : 1);
        pairs_.resize(nbTileRows_ * nbTileCols_, {false, nullptr});
    }

    std::pair<bool, std::shared_ptr<PairTile<T>>> &pairsAt(size_t x, size_t y) {
        return pairs_[y * nbTileCols_ + x];
    }

    bool pairValid(size_t x, size_t y) {
        auto pair = pairsAt(x, y);

        // the pair is not null and has never been sent
        if (pair.first || !pair.second) {
            return false;
        }
        // the pair is ready (surrounding pairs are not null)
        for (auto [modx, mody] : mods) {
            size_t x2 = x + modx;
            size_t y2 = y + mody;
            if (x2 < nbTileCols_ && y2 < nbTileRows_ &&
                !pairsAt(x2, y2).second) {
                return false;
            }
        }
        return true;
    }

    void execute(std::shared_ptr<PairTile<T>> pair) override {
        if (pairs_.empty()) {
            initPairs(pair->first);
        }
        size_t x = pair->first->x() / pair->first->tileSize();
        size_t y = pair->first->y() / pair->first->tileSize();
        pairsAt(x, y).second = pair;

        for (auto [modx, mody] : mods) {
            size_t x2 = x + modx;
            size_t y2 = y + mody;
            if (x2 < nbTileCols_ && y2 < nbTileRows_ && pairValid(x2, y2)) {
                this->addResult(pairsAt(x2, y2).second);
                pairsAt(x2, y2).first = true;
            }
        }
    }

  private:
    size_t nbTileRows_ = 0;
    size_t nbTileCols_ = 0;
    std::vector<std::pair<
        bool, std::shared_ptr<PairTile<T>>>>
        pairs_ = {};
    static constexpr std::pair<int, int> mods[9]{
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0},
        {0, 1},   {1, -1}, {1, 0},  {1, 1},
    };
};

} // end namespace hhimg

#endif
