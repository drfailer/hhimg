#ifndef HEDGEHOG_PIPELINE_H
#define HEDGEHOG_PIPELINE_H
#include "../abstract/abstract_algorithm.h"
#include "../abstract/data/abstract_tile.h"
#include "../algorithm/tile/copy.h"
#include "../tools/null_type.h"
#include "hhimg/algorithm/tile/split_graph/split_graph.h"
#include <hedgehog/hedgehog.h>

namespace hhimg {

template <typename T>
using TilePipeline = hh::Graph<1, AbstractImage<T>, AbstractTile<T>>;

template <typename T>
using GraphType = hh::Graph<1, AbstractTile<T>, AbstractTile<T>>;

template <typename T>
using TaskType = hh::AbstractTask<1, AbstractTile<T>, AbstractTile<T>>;

template <typename T, typename LastTask = NullType>
struct HedgehogPipeline : TilePipeline<T> {
    HedgehogPipeline(size_t tileSize, size_t splitThreads, size_t copyThreads,
                     std::shared_ptr<AbstractTileFactory<T>> tileFactory,
                     std::string graphName = "unamed pipeline")
        : tileSize_(tileSize), tileFactory_(tileFactory),
          splitThreads_(splitThreads), copyThreads_(copyThreads),
          graph_(std::make_shared<TilePipeline<T>>(std::move(graphName))) {}

    HedgehogPipeline(size_t tileSize, size_t splitThreads, size_t copyThreads,
                     LastTask lastTask,
                     std::shared_ptr<AbstractTileFactory<T>> tileFactory,
                     std::shared_ptr<TilePipeline<T>> graph)
        : lastTask_(lastTask), tileSize_(tileSize), tileFactory_(tileFactory),
          splitThreads_(splitThreads), copyThreads_(copyThreads),
          graph_(graph) {}

    Image<T> operator()(Image<T> image) {
        setupGraph();
        graph_->pushData(image);
        graph_->finishPushingData();
        graph_->waitForTermination();
        graph_->createDotFile("graph.dot", hh::ColorScheme::EXECUTION,
                              hh::StructureOptions::QUEUE);
        return image;
    }

    void setupGraph() {
        if (!graphCompleted_) {
            setOutput();
            graph_->executeGraph();
            graphCompleted_ = true;
        } else {
            graph_->cleanGraph();
        }
    }

    template <typename Algo>
    std::shared_ptr<HedgehogPipeline<T, Algo>> add(Algo algo) {
        if constexpr (null_type_v<LastTask>) {
            auto splitGraph = std::make_shared<SplitGraph<T>>(
                splitThreads_, tileSize_, ghostRegionSize_, tileFactory_);
            graph_->inputs(splitGraph);
            graph_->edges(splitGraph, algo);
        } else {
            graph_->edges(lastTask_, algo);
        }
        return std::make_shared<HedgehogPipeline<T, Algo>>(
            tileSize_, splitThreads_, copyThreads_, algo, tileFactory_, graph_);
    }

    // can be overriden if a ghost region is required
    size_t ghostRegionSize() const { return ghostRegionSize_; }
    void ghostRegionSize(size_t size) { ghostRegionSize_ = size; }

  private:
    void setOutput() {
        auto copy = std::make_shared<Copy<T>>(copyThreads_, tileFactory_);
        if constexpr (!null_type_v<LastTask>) {
            graph_->edges(lastTask_, copy);
        } // else throw
        graph_->outputs(copy);
    }

  private:
    bool graphCompleted_ = false;
    LastTask lastTask_;
    size_t tileSize_ = 0;
    size_t ghostRegionSize_ = 0;
    size_t splitThreads_ = 1;
    size_t copyThreads_ = 1;
    std::shared_ptr<AbstractTileFactory<T>> tileFactory_ = nullptr;
    std::shared_ptr<TilePipeline<T>> graph_ = nullptr;
};

} // namespace hhimg

#endif
