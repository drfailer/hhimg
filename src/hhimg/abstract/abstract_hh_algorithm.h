#ifndef HEDGEHOG_ALGORITHM_H
#define HEDGEHOG_ALGORITHM_H
#include "../abstract/abstract_tile_algorithm.h"
#include "../abstract/abstract_algorithm.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
using GraphType = hh::Graph<1, AbstractTile<T>, AbstractTile<T>>;

template <typename T> struct AbstractHHAlgorithm : GraphType<T> {
    AbstractHHAlgorithm(std::string graphName = "unamed")
        : GraphType<T>(graphName) {}
    virtual ~AbstractHHAlgorithm() {}

    virtual Image<T> operator()(Image<T> image) = 0;

    void compile() {
        this->inputs(firstTask_);
        this->outputs(lastTask_);
    }

    void push_front(std::shared_ptr<AbstractTileAlgorithm<T>> algo) {
        if (firstTask_ == nullptr) {
            firstTask_ = algo;
            lastTask_ = algo;
        } else {
            this->edges(algo, firstTask_);
            firstTask_ = algo;
        }
    }

    void push_back(std::shared_ptr<AbstractTileAlgorithm<T>> algo) {
        if (lastTask_ == nullptr) {
            firstTask_ = algo;
            lastTask_ = algo;
        } else {
            this->edges(lastTask_, algo);
            lastTask_ = algo;
        }
    }

  private:
    std::shared_ptr<hh::AbstractTask<1, AbstractTile<T>, AbstractTile<T>>>
        firstTask_ = nullptr;
    std::shared_ptr<hh::AbstractTask<1, AbstractTile<T>, AbstractTile<T>>>
        lastTask_ = nullptr;
};

} // namespace hhimg

#endif
