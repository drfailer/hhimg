#ifndef HEDGEHOG_PIPELINE_H
#define HEDGEHOG_PIPELINE_H
#include "../abstract/abstract_algorithm.h"
#include "../abstract/data/abstract_tile.h"
#include "../algorithm/tile/copy.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
using TilePipeline = hh::Graph<1, AbstractImage<T>, AbstractTile<T>>;

template <typename T>
using GraphType = hh::Graph<1, AbstractTile<T>, AbstractTile<T>>;

template <typename T>
using TaskType = hh::AbstractTask<1, AbstractTile<T>, AbstractTile<T>>;

template <typename T> struct HedgehogPipeline : TilePipeline<T> {
    HedgehogPipeline(std::string graphName = "unamed pipeline")
        : graph_(std::make_shared<TilePipeline<T>>(graphName)) {}
    ~HedgehogPipeline() {}

    Image<T> operator()(Image<T> image) {
        compile();
        graph_->pushData(image);
        graph_->finishPushingData();
        graph_->waitForTermination();
        graph_->createDotFile("graph.dot", hh::ColorScheme::EXECUTION,
                              hh::StructureOptions::QUEUE);
        return image;
    }

    void compile() {
        auto copy = std::make_shared<Copy<T>>(1);
        createEdge(copy);
        graph_->outputs(copy);
        graph_->executeGraph();
    }

    void push_back(auto algo) {
        createEdge(algo);
        lastTask_ = algo;
    }

    void createEdge(auto algo) {
        if (lastTask_.taskTileTile) {
            graph_->edges(lastTask_.taskTileTile, algo);
        } else {
            graph_->edges(lastTask_.graphTileTile, algo);
        }
    }

    // can be overriden if a ghost region is required
    size_t ghostRegionSize() const { return ghostRegionSize_; }
    void ghostRegionSize(size_t size) { ghostRegionSize_ = size; }

    std::shared_ptr<TilePipeline<T>> graph() { return graph_; }
    void lastTask(auto algo) { lastTask_ = algo; }

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
    Tasks lastTask_;
    size_t ghostRegionSize_ = 0;
    std::shared_ptr<TilePipeline<T>> graph_ = nullptr;
};

} // namespace hhimg

#endif
