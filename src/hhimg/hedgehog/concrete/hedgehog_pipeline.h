#ifndef HEDGEHOG_PIPELINE_H
#define HEDGEHOG_PIPELINE_H
#include "../../tools/null_type.h"
#include "../abstract/data/abstract_tile.h"
#include "../algorithm/tile/copy.h"
#include "../algorithm/tile/split_graph/split_graph.h"
#include <hedgehog/hedgehog.h>

namespace hhimg::hdg {

template <typename T>
using TilePipeline = hh::Graph<1, AbstractImage<T>, AbstractTile<T>>;

template <typename T>
using GraphType = hh::Graph<1, AbstractTile<T>, AbstractTile<T>>;

template <typename T>
using TaskType = hh::AbstractTask<1, AbstractTile<T>, AbstractTile<T>>;

template <typename T, typename FirstTask = NullType,
          typename LastTask = NullType>
struct HedgehogPipeline {
    HedgehogPipeline(size_t tileSize, size_t splitThreads, size_t copyThreads,
                     std::shared_ptr<AbstractTileFactory<T>> tileFactory,
                     std::string graphName = "unamed pipeline")
        : tileSize_(tileSize), tileFactory_(tileFactory),
          splitThreads_(splitThreads), copyThreads_(copyThreads),
          graph_(std::make_shared<TilePipeline<T>>(std::move(graphName))) {}

    HedgehogPipeline(size_t tileSize, size_t ghostRegionSize,
                     size_t splitThreads, size_t copyThreads,
                     FirstTask firstTask, LastTask lastTask,
                     std::shared_ptr<AbstractTileFactory<T>> tileFactory,
                     std::shared_ptr<TilePipeline<T>> graph)
        : firstTask_(firstTask), lastTask_(lastTask), tileSize_(tileSize),
          ghostRegionSize_(ghostRegionSize), tileFactory_(tileFactory),
          splitThreads_(splitThreads), copyThreads_(copyThreads),
          graph_(graph) {}

    HedgehogPipeline(HedgehogPipeline<T, FirstTask, LastTask> const &other)
        : HedgehogPipeline(other.tileSize_, other.ghostRegionSize_,
                           other.splitThreads_, other.copyThreads_,
                           other.firstTask_, other.lastTask_,
                           other.tileFactory_, other.graph_) {}

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
            setInput();
            setOutput();
            graph_->executeGraph();
            graphCompleted_ = true;
        } else {
            graph_->cleanGraph();
        }
    }

    template <typename Algo> auto add(Algo algo) {
        if constexpr (null_type_v<LastTask>) {
            return std::make_shared<HedgehogPipeline<T, Algo, Algo>>(
                tileSize_, ghostRegionSize_, splitThreads_, copyThreads_, algo,
                algo, tileFactory_, graph_);
        } else {
            graph_->edges(lastTask_, algo);
            return std::make_shared<HedgehogPipeline<T, FirstTask, Algo>>(
                tileSize_, ghostRegionSize_, splitThreads_, copyThreads_,
                firstTask_, algo, tileFactory_, graph_);
        }
    }

    // can be overriden if a ghost region is required
    size_t ghostRegionSize() const { return ghostRegionSize_; }
    void ghostRegionSize(size_t size) { ghostRegionSize_ = size; }

    std::shared_ptr<SplitGraph<T>> createSplitGraph() const {
        return std::make_shared<SplitGraph<T>>(splitThreads_, tileSize_,
                                               ghostRegionSize_, tileFactory_);
    }

    std::shared_ptr<SplitGraph<T>>
    createSplitGraph(size_t otherGhostRegionSize) const {
        return std::make_shared<SplitGraph<T>>(
            splitThreads_, tileSize_, otherGhostRegionSize, tileFactory_);
    }

    void edges(auto t1, auto t2) { graph_->edges(t1, t2); }
    void inputs(auto elt) { graph_->inputs(elt); }
    void outputs(auto elt) { graph_->outputs(elt); }
    auto graph() { return graph_; }
    FirstTask firstTask() { return firstTask_; }
    LastTask lastTask() { return lastTask_; }

  private:
    void setInput() {
        if constexpr (!null_type_v<FirstTask>) {
            auto splitGraph = createSplitGraph();
            graph_->inputs(splitGraph);
            graph_->edges(splitGraph, firstTask_);
        } else {
            throw std::runtime_error("error: empty pipeline.");
        }
    }

    void setOutput() {
        if constexpr (!null_type_v<LastTask>) {
            auto copy = std::make_shared<Copy<T>>(copyThreads_, tileFactory_);
            graph_->edges(lastTask_, copy);
            graph_->outputs(copy);
        } else {
            throw std::runtime_error("error: empty pipeline.");
        }
    }

  private:
    bool graphCompleted_ = false;
    LastTask lastTask_;
    FirstTask firstTask_;
    size_t tileSize_ = 0;
    size_t ghostRegionSize_ = 0;
    size_t splitThreads_ = 1;
    size_t copyThreads_ = 1;
    std::shared_ptr<AbstractTileFactory<T>> tileFactory_ = nullptr;
    std::shared_ptr<TilePipeline<T>> graph_ = nullptr;
};

} // namespace hhimg::hdg

#endif
