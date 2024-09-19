#ifndef HEDGEHOG_ALGORITHM_H
#define HEDGEHOG_ALGORITHM_H
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_tile_algorithm.h"
#include "../algorithm/tile/copy.h"
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
        auto copy = std::make_shared<Copy<T>>(1);
        createEdge(copy);
        this->outputs(copy);
    }

    void push_back(auto algo) {
        if (lastTask_.null()) {
            this->inputs(algo);
            lastTask_ = algo;
        } else {
            createEdge(algo);
            lastTask_ = algo;
        }
    }

    void createEdge(auto algo) {
        if (lastTask_.taskTileTile) {
            this->edges(lastTask_.taskTileTile, algo);
        } else {
            this->edges(lastTask_.graphTileTile, algo);
        }
    }

    // can be overriden if a ghost region is required
    size_t ghostRegionSize() const { return ghostRegionSize_; }
    void ghostRegionSize(size_t size) { ghostRegionSize_ = size; }

  private:
    struct Tasks {
        std::shared_ptr<TaskType<T>> taskTileTile = nullptr;
        std::shared_ptr<GraphType<T>> graphTileTile = nullptr;

        bool null() const { return !taskTileTile && !graphTileTile; }

        Tasks &operator=(std::shared_ptr<TaskType<T>> task) {
            graphTileTile = nullptr;
            taskTileTile = task;
            return *this;
        }

        Tasks &operator=(std::shared_ptr<GraphType<T>> graph) {
            taskTileTile = nullptr;
            graphTileTile = graph;
            return *this;
        }
    };

  private:
    // todo: use a variant Task<tile, tile> / Graph<tile, tile>
    Tasks lastTask_;
    size_t ghostRegionSize_ = 0;
};

} // namespace hhimg

#endif
