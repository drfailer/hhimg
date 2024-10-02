#ifndef UPDATE_STENCILS_TASK_H
#define UPDATE_STENCILS_TASK_H
#include "../../../abstract/tile_algorithms.h"
#include "hedgehog/src/api/task/abstract_task.h"
#include <hedgehog/hedgehog.h>

namespace hhimg::hdg {

template <typename T>
using UpdateStencilsTaskType = hh::AbstractTask<1, PairTile<T>, PairTile<T>>;

template <typename T> struct UpdateStencilsTask : UpdateStencilsTaskType<T> {
    UpdateStencilsTask(size_t nbThreads)
        : hh::AbstractTask<1, PairTile<T>, PairTile<T>>("Update Stencils Task",
                                                        nbThreads) {}

    void execute(std::shared_ptr<PairTile<T>> tiles) {
        auto t1 = tiles->first;
        auto t2 = tiles->second;
        size_t x1Begin, x1End, y1Begin, y1End; // t1 ghost region boundaries
        size_t x2Begin, x2End, y2Begin, y2End; // t2 ghost region boundaries
        int modx1, mody1, modx2, mody2;        // ghost region -> tile mods
        size_t x1, x2, y1, y2;
        bool right = t1->x() < t2->x();
        bool left = t1->x() > t2->x();
        bool up = t1->y() > t2->y();
        bool bottom = t1->y() < t2->y();

        if (right) {
            x1Begin = t1->ghostWidth() - t1->ghostRegionSize();
            x1End = t1->ghostWidth();
            modx1 = -t1->ghostRegionSize();
            x2Begin = 0;
            x2End = t2->ghostRegionSize();
            modx2 = t2->ghostRegionSize();
        } else if (left) {
            x1Begin = 0;
            x1End = t1->ghostRegionSize();
            modx1 = t1->ghostRegionSize();
            x2Begin = t2->ghostWidth() - t2->ghostRegionSize();
            x2End = t2->ghostWidth();
            modx2 = -t2->ghostRegionSize();
        } else {
            x1Begin = t1->ghostRegionSize();
            x1End = t1->ghostWidth() - t1->ghostRegionSize();
            modx1 = 0;
            x2Begin = t2->ghostRegionSize();
            x2End = t2->ghostWidth() - t2->ghostRegionSize();
            modx2 = 0;
        }

        if (up) {
            y1Begin = 0;
            y1End = t1->ghostRegionSize();
            mody1 = t1->ghostRegionSize();
            y2Begin = t2->ghostHeight() - t2->ghostRegionSize();
            y2End = t2->ghostHeight();
            mody2 = -t2->ghostRegionSize();
        } else if (bottom) {
            y1Begin = t1->ghostHeight() - t1->ghostRegionSize();
            y1End = t1->ghostHeight();
            mody1 = -t1->ghostRegionSize();
            y2Begin = 0;
            y2End = t2->ghostRegionSize();
            mody2 = t2->ghostRegionSize();
        } else {
            y1Begin = t1->ghostRegionSize();
            y1End = t1->ghostHeight() - t1->ghostRegionSize();
            mody1 = 0;
            y2Begin = t2->ghostRegionSize();
            y2End = t2->ghostHeight() - t2->ghostRegionSize();
            mody2 = 0;
        }

        // assert for security
        assert((x1End - x1Begin) == (x2End - x2Begin));
        assert((y1End - y1Begin) == (y2End - y2Begin));

        // update the tiles' stencils
        for (y1 = y1Begin, y2 = y2Begin; y1 < y1End; ++y1, ++y2) {
            for (x1 = x1Begin, x2 = x2Begin; x1 < x1End; ++x1, ++x2) {
                t1->ghostSet(x1, y1, t2->ghostAt(x2 + modx2, y2 + mody2));
                t2->ghostSet(x2, y2, t1->ghostAt(x1 + modx1, y1 + mody1));
            }
        }
        this->addResult(tiles);
    }

    std::shared_ptr<UpdateStencilsTaskType<T>> copy() {
        return std::make_shared<UpdateStencilsTask<T>>(this->numberThreads());
    }
};

} // end namespace hhimg::hdg

#endif
