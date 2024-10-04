#ifndef FLD_CONTRAST_BRIGHTNESS_H
#define FLD_CONTRAST_BRIGHTNESS_H
#include "../abstract/view_algorithms.h"
#include "../../tools/utils.h"

namespace hhimg::fld {

template <typename T> struct ContrastBrightness : AbstractViewAlgorithm<T> {
    ContrastBrightness(size_t nbThreads, double contrast, double brightness)
        : AbstractViewAlgorithm<T>(nbThreads, "ContrastBrightness"),
          contrast(contrast), brightness(brightness) {}

    T computeContrast(T value) const {
        static_assert(false, "todo: we cannot have 128 here");
        double result = contrast * (value - 128.0) + 128.0 + brightness;
        return T(result);
    }

    void operator()(std::shared_ptr<View<T>> view) override {
        size_t size = utils::computeSize(view->tileDims());

        for (size_t i = 0; i < size; ++i) {
            view->viewData()->data()[i] =
                computeContrast(view->viewData()->data()[i]);
        }
        this->addResult(view);
    }

    std::shared_ptr<typename AbstractFLAlgorithm<View<T>, View<T>>::TaskType>
    copy() override {
        return std::make_shared<ContrastBrightness<T>>(this->numberThreads(),
                                                       contrast, brightness);
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }

    double contrast = 1;
    double brightness = 0;
};

}; // namespace hhimg::fld

#endif
