#ifndef TEST_ALGORITHM_H
#define TEST_ALGORITHM_H
#include "../../abstract/abstract_fl_algorithm.h"
#include "../../concrete/data/fl_view.h"
#include <fast_loader/fast_loader.h>

namespace hhimg::fld {

template <typename T>
struct TestAlgorithm : AbstractFLAlgorithm<View<T>, View<T>> {

    TestAlgorithm(size_t nbThreads)
        : AbstractFLAlgorithm<View<T>, View<T>>(nbThreads, "test") {}

    void operator()(std::shared_ptr<View<T>> in) override {
        this->addResult(in);
        in->returnToMemoryManager();
    }

    std::shared_ptr<typename AbstractFLAlgorithm<View<T>, View<T>>::TaskType>
    copy() override {
        return std::make_shared<TestAlgorithm<T>>(this->numberThreads());
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }
};

} // end namespace hhimg::fld

#endif
