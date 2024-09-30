#ifndef TEST_ALGORITHM_H
#define TEST_ALGORITHM_H
#include "../../abstract/fast_loader/abstract_fl_algorithm.h"
#include <fast_loader/fast_loader.h>

namespace hhimg {

template <typename T>
struct TestAlgorithm
    : AbstractFLAlgorithm<fl::DefaultView<T>, fl::DefaultView<T>> {
    void operator()(std::shared_ptr<fl::DefaultView<T>> in) override {
        std::cout << "tile received" << std::endl;
        this->addResult(in);
    }

    constexpr static auto setup(auto pipeline, auto self) {
        return pipeline->add(self);
    }
};

} // end namespace hhimg

#endif
