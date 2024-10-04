#ifndef FLD_GRAY_SCALE_HPP
#define FLD_GRAY_SCALE_HPP
#include "../abstract/view_algorithms.h"
#include "../../tools/utils.h"

namespace hhimg::fld {

template <typename T> struct GrayScale : AbstractViewAlgorithm<T> {
    GrayScale(size_t nbThreads)
        : AbstractViewAlgorithm<T>(nbThreads, "Grayscale") {}

    void operator()(std::shared_ptr<View<T>> view) override {
        static_assert(false, "not implemented");
        size_t size = utils::computeSize(view->tileDims());

        for (size_t i = 0; i < size; ++i) {
            view->viewData()->data()[i] = view->viewData()->data()[i];
        }
        this->addResult(view);
    }

    std::shared_ptr<typename AbstractFLAlgorithm<View<T>, View<T>>::TaskType>
    copy() override {
        return std::make_shared<GrayScale<T>>(this->numberThreads());
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }
};

}; // namespace hhimg::fld

#endif
