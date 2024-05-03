#ifndef CHAIN_ALGORITHM_HPP
#define CHAIN_ALGORITHM_HPP
#include "../abstract/abstract_algorithm.h"
#include <memory>

namespace hhimg {

template <typename T> struct ChainAlgorithm : public AbstractAlgorithm<T> {
    ChainAlgorithm(AbstractAlgorithm<T> const &algorithm1, AbstractAlgorithm<T> const &algorithm2)
        : algorithm1_(algorithm1), algorithm2_(algorithm2) {}

    const std::shared_ptr<AbstractImage<T>>
    operator()(std::shared_ptr<AbstractImage<T>> image) const override {
        return algorithm2_(algorithm1_(image));
    }

  private:
    AbstractAlgorithm<T>& algorithm1_;
    AbstractAlgorithm<T>& algorithm2_;
};

template <typename T>
ChainAlgorithm<T> operator|(AbstractAlgorithm<T> &algorithm1,
               AbstractAlgorithm<T> const &algorithm2) {
    return ChainAlgorithm<T>(algorithm1, algorithm2);
}

} // namespace hhimg

#endif
