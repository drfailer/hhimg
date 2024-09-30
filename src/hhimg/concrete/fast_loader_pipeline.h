#ifndef FAST_LOADER_PIPELINE_H
#define FAST_LOADER_PIPELINE_H
#include "../tools/null_type.h"
#include "hhimg/concrete/data/fl_img.h"
#include <fast_loader/fast_loader.h>
#include <hedgehog/hedgehog.h>
#include <memory>

namespace hhimg {

template <typename T>
using ViewPipeline = hh::Graph<1, fl::DefaultView<T>, fl::DefaultView<T>>;

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

    template <typename TileLoader>
    void operator()(FLImg<TileLoader> image) {
        if constexpr (!null_type_v<LastTask>) {
        setupGraph();
        auto config =
            std::make_unique<fl::FastLoaderConfiguration<fl::DefaultView<T>>>(
                image.tileLoader);
        // todo: config (border size?)
        // Create the Fast Loader Graph
        fastLoader_ = std::make_shared<fl::FastLoaderGraph<fl::DefaultView<T>>>(
            std::move(config));
        graph_->edges(fastLoader_, firstTask_);

        fastLoader_->requestAllViews();
        graph_->waitForTermination();
        graph_->createDotFile("graph.dot", hh::ColorScheme::EXECUTION,
                              hh::StructureOptions::QUEUE);
        } else {
          throw std::runtime_error("error: empty pipeline.");
        }
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

    template <typename Algo> auto add(Algo algo) {
        if constexpr (null_type_v<LastTask>) {
            graph_->inputs(algo);
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
    void setOutput() {
        // TODO
        /* auto copy = std::make_shared<Copy<T>>(copyThreads_); */
        /* if constexpr (!null_type_v<LastTask>) { */
        /*     graph_->edges(lastTask_, copy); */
        /* } // else throw */
        /* graph_->outputs(copy); */
    }

  private:
    bool graphCompleted_ = false;
    FirstTask firstTask_;
    LastTask lastTask_;
    size_t tileSize_ = 0;
    size_t ghostRegionSize_ = 0;
    std::shared_ptr<ViewPipeline<T>> graph_ = nullptr;
    std::shared_ptr<fl::FastLoaderGraph<fl::DefaultView<T>>> fastLoader_ =
        nullptr;
};

} // end namespace hhimg

#endif
