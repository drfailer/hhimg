#ifndef HEDGEHOG_ALGORITHM_H
#define HEDGEHOG_ALGORITHM_H
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_tile_algorithm.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
using GraphType = hh::Graph<1, AbstractTile<T>, AbstractTile<T>>;

template <typename T> struct AbstractHHAlgorithm : GraphType<T> {
    AbstractHHAlgorithm(std::string graphName = "unamed")
        : GraphType<T>(graphName) {}
    virtual ~AbstractHHAlgorithm() {}

    virtual Image<T> operator()(Image<T> image) = 0;

    void compile() { this->outputs(lastTask_); }

    void push_back(auto algo) {
        if (lastTask_ == nullptr) {
            this->inputs(algo);
            lastTask_ = algo;
        } else {
            this->edges(lastTask_, algo);
            lastTask_ = algo;
        }
    }

    // can be overriden if a ghost region is required
    size_t ghostRegionSize() const { return ghostRegionSize_; }
    void ghostRegionSize(size_t size) { ghostRegionSize_ = size; }

    std::shared_ptr<TaskType<T>> lastTask() const { return lastTask_; }
    void lastTask(std::shared_ptr<TaskType<T>> lastTask) {
        lastTask_ = lastTask;
    }

  private:
    std::shared_ptr<TaskType<T>> lastTask_ = nullptr;
    size_t ghostRegionSize_ = 0;
};

} // namespace hhimg

#endif
