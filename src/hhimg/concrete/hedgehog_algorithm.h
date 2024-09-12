#ifndef HEDGEHOG_ALGORITHM_H
#define HEDGEHOG_ALGORITHM_H
#include "../abstract/abstract_tile_algorithm.h"
#include "../abstract/abstract_algorithm.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T> struct HedgehogAlgorithm {
    using GraphType = hh::Graph<1, AbstractTile<T>, AbstractTile<T>>;

    HedgehogAlgorithm(std::string graphName = "unamed")
        : graph_(std::make_shared<GraphType>(std::move(graphName))) {}
    /* HedgehogAlgorithm(HedgehogAlgorithm) {} */
    virtual ~HedgehogAlgorithm() {}

    virtual ImgData<T> operator()(ImgData<T> image) = 0;

    std::shared_ptr<GraphType> graph() { return graph_; }

    void compile() {
        graph_->inputs(firstTask_);
        graph_->outputs(lastTask_);
    }

    void push_front(std::shared_ptr<AbstractTileAlgorithm<T>> algo) {
        if (firstTask_ == nullptr) {
            firstTask_ = algo;
            lastTask_ = algo;
        } else {
            graph_->edges(algo, firstTask_);
            firstTask_ = algo;
        }
    }

    void push_back(std::shared_ptr<AbstractTileAlgorithm<T>> algo) {
        if (lastTask_ == nullptr) {
            firstTask_ = algo;
            lastTask_ = algo;
        } else {
            graph_->edges(lastTask_, algo);
            lastTask_ = algo;
        }
    }

  private:
    std::shared_ptr<GraphType> graph_ = nullptr;
    std::shared_ptr<hh::AbstractTask<1, AbstractTile<T>, AbstractTile<T>>>
        firstTask_ = nullptr;
    std::shared_ptr<hh::AbstractTask<1, AbstractTile<T>, AbstractTile<T>>>
        lastTask_ = nullptr;
};

} // namespace hhimg

#endif
