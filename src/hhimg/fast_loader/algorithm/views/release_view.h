#ifndef RELEASE_VIEW_H
#define RELEASE_VIEW_H
#include "../../concrete/data/fl_view.h"
#include <fast_loader/fast_loader.h>
#include <hedgehog/hedgehog.h>

namespace hhimg::fld {

template <typename T, typename TileWriter>
struct ReleaseView : hh::AbstractTask<1, View<T>, View<T>> {
    ReleaseView(TileWriter tr)
        : hh::AbstractTask<1, View<T>, View<T>>("Release Views"),
        tr(tr) {}

    void execute(std::shared_ptr<View<T>> in) override {
        std::shared_ptr<View<T>> out = nullptr;
        this->addResult(out); // FIXME: not sure about that
        tr->writeTileToFile(in);
        in->returnToMemoryManager();
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }

    TileWriter tr;
};

} // end namespace hhimg::fld

#endif
