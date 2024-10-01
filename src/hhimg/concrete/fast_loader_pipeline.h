#ifndef FAST_LOADER_PIPELINE_H
#define FAST_LOADER_PIPELINE_H
#include "../concrete/data/fl_img.h"
#include "../concrete/data/fl_view.h"
#include "../tools/null_type.h"
#include <fast_loader/fast_loader.h>
#include <hedgehog/hedgehog.h>
#include <memory>

namespace hhimg {

template <typename T>
using ViewPipeline = hh::Graph<1, fl::IndexRequest, View<T>>;

template <typename T, typename FirstTask = NullType,
          typename LastTask = NullType>
struct FastLoaderPipeline {
    FastLoaderPipeline(size_t tileSize,
                       std::string graphName = "unamed pipeline")
        : tileSize_(tileSize),
          graph_(std::make_shared<ViewPipeline<T>>(std::move(graphName))) {}

    FastLoaderPipeline(size_t tileSize, size_t ghostRegionSize,
                       FirstTask firstTask, LastTask lastTask,
                       std::shared_ptr<ViewPipeline<T>> graph)
        : firstTask_(firstTask), lastTask_(lastTask), tileSize_(tileSize),
          ghostRegionSize_(ghostRegionSize), graph_(graph) {}

    template <typename TileLoader> void operator()(FLImg<TileLoader> in) {
        if constexpr (!null_type_v<LastTask>) {
            setupGraph(in);
            requestAllViews(0);
            graph_->finishPushingData();
            graph_->waitForTermination();
            graph_->createDotFile("graph.dot", hh::ColorScheme::EXECUTION,
                                  hh::StructureOptions::QUEUE);
        } else {
            throw std::runtime_error("error: empty pipeline.");
        }
    }

    void setupGraph(auto in) {
        if (!graphCompleted_) {
            setInput(in);
            setOutput();
            graph_->executeGraph();
            graphCompleted_ = true;
        } else {
            graph_->cleanGraph();
        }
    }

    void requestAllViews(size_t level) {
        for (std::shared_ptr<fl::IndexRequest> const &indexRequest :
             fastLoader_->generateIndexRequestForAllViews(level)) {
            graph_->pushData(indexRequest);
        }
    }

    template <typename Algo> auto add(Algo algo) {
        if constexpr (null_type_v<LastTask>) {
            return std::make_shared<FastLoaderPipeline<T, Algo, Algo>>(
                tileSize_, ghostRegionSize_, algo, algo, graph_);
        } else {
            graph_->edges(lastTask_, algo);
            return std::make_shared<FastLoaderPipeline<T, FirstTask, Algo>>(
                tileSize_, ghostRegionSize_, firstTask_, algo, graph_);
        }
    }

    // can be overriden if a ghost region is required
    size_t ghostRegionSize() const { return ghostRegionSize_; }
    void ghostRegionSize(size_t size) { ghostRegionSize_ = size; }

  private:
    template <typename TileLoader> void setInput(FLImg<TileLoader> in) {
        auto config = std::make_unique<fl::FastLoaderConfiguration<View<T>>>(
            in.tileLoader);
        fastLoader_ =
            std::make_shared<fl::FastLoaderGraph<View<T>>>(std::move(config));
        graph_->inputs(fastLoader_);
        graph_->edges(fastLoader_, firstTask_);
    }

    void setOutput() { graph_->outputs(lastTask_); }

  private:
    bool graphCompleted_ = false;
    FirstTask firstTask_;
    LastTask lastTask_;
    size_t tileSize_ = 0;
    size_t ghostRegionSize_ = 0;
    std::shared_ptr<ViewPipeline<T>> graph_ = nullptr;
    std::shared_ptr<fl::FastLoaderGraph<View<T>>> fastLoader_ = nullptr;
};

} // end namespace hhimg

#endif
