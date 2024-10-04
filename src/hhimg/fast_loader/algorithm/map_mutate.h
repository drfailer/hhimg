#ifndef FLD_MAP_MUTATE_HPP
#define FLD_MAP_MUTATE_HPP
#include "../../tools/utils.h"
#include "../abstract/view_algorithms.h"
#include "../concrete/data/fl_view.h"
#include <functional>

namespace hhimg::fld {

template <typename T> struct MapMutate : AbstractViewAlgorithm<T> {
    using ComputeFn = std::function<T(std::shared_ptr<View<T>>, size_t idx)>;

    MapMutate(size_t nbThreads, ComputeFn compute)
        : AbstractViewAlgorithm<T>(nbThreads, "MapMutate"), compute_(compute) {}

    void operator()(std::shared_ptr<View<T>> view) override {
        size_t size = utils::computeSize(view->tileDims());

        for (size_t i = 0; i < size; ++i) {
            view->viewData()->data()[i] = compute_(view->viewData()->data(), i);
        }
        this->addResult(view);
    }

    std::shared_ptr<typename AbstractViewAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<MapMutate<T>>(this->numberThreads(), compute_);
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }

  private:
    ComputeFn compute_;
};

} // namespace hhimg::fld

#endif
