#ifndef RELEASE_VIEW_H
#define RELEASE_VIEW_H
#include "../../concrete/data/fl_view.h"
#include <fast_loader/fast_loader.h>
#include <hedgehog/hedgehog.h>

namespace hhimg::fld {

template <typename T>
struct ReleaseView : hh::AbstractTask<1, View<T>, View<T>> {
    ReleaseView(size_t nbThreads)
        : hh::AbstractTask<1, View<T>, View<T>>("Release Views", nbThreads) {}

    void execute(std::shared_ptr<View<T>> in) override {
        this->addResult(in); // FIXME: not sure about that
        in->returnToMemoryManager();
    }

    std::shared_ptr<hh::AbstractTask<1, View<T>, View<T>>> copy() override {
        return std::make_shared<ReleaseView<T>>(this->numberThreads());
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }
};

} // end namespace hhimg::fld

#endif
