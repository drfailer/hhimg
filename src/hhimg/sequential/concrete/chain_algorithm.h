#ifndef CHAIN_ALGORITHM_HPP
#define CHAIN_ALGORITHM_HPP
#include "../abstract/abstract_algorithm.h"

namespace hhimg::seq {

template <typename T> struct ChainAlgorithm : public AbstractAlgorithm<T> {
    ChainAlgorithm(AbstractAlgorithm<T> const &algorithm1,
                   AbstractAlgorithm<T> const &algorithm2)
        : algorithm1_(algorithm1), algorithm2_(algorithm2) {}

    Image<T> operator()(Image<T> image) const override {
        return algorithm2_(algorithm1_(image));
    }

  private:
    AbstractAlgorithm<T> const &algorithm1_;
    AbstractAlgorithm<T> const &algorithm2_;
};

} // namespace hhimg::seq

#endif
