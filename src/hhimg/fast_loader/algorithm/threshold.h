#ifndef FLD_THRESHOLD_HPP
#define FLD_THRESHOLD_HPP
#include "../abstract/view_algorithms.h"
#include "../../tools/utils.h"

namespace hhimg::fld {

template <typename T> struct Threshold : AbstractViewAlgorithm<T> {
    Threshold(size_t nbThreads, T value, T low = 0, T high = 255)
        : AbstractViewAlgorithm<T>(nbThreads, "Threshold"), value(value),
          low(low), high(high) {}

    void operator()(std::shared_ptr<View<T>> view) override {
        size_t size = utils::computeSize(view->tileDims());

        for (size_t i = 0; i < size; ++i) {
            if (view->viewData()->data()[i] < value) {
                view->viewData()->data()[i] = low;
            } else {
                view->viewData()->data()[i] = high;
            }
        }
        this->addResult(view);
    }

    std::shared_ptr<typename AbstractFLAlgorithm<View<T>, View<T>>::TaskType>
    copy() override {
        return std::make_shared<Threshold<T>>(this->numberThreads(), value, low,
                                              high);
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }

    // TODO
    T low = 0;
    T high = 255;
    T value = 0;
};

} // namespace hhimg::fld

#endif
