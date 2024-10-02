#ifndef HDG_MINUS_HPP
#define HDG_MINUS_HPP
#include "../../tools/hhutils/trigger_state.h"
#include "../../tools/null_type.h"
#include "../abstract/tile_algorithms.h"
#include "../concrete/hedgehog_pipeline.h"
#include "tile/minus/minus_state.h"
#include "tile/minus/minus_state_manager.h"
#include "tile/minus/minus_wrapper.h"

namespace hhimg::hdg {

template <typename T, typename F = NullType, typename L = NullType>
class Minus : public Abstract2TilesAlgorithm<T> {
  public:
    Minus(size_t nbThreads, Image<T> const &imageToSubstract,
          std::shared_ptr<HedgehogPipeline<T, F, L>> pipeline = nullptr)
        : Abstract2TilesAlgorithm<T>(nbThreads, "Minus"),
          imageToSubstract_(imageToSubstract), pipeline_(pipeline) {}
    Minus(Image<T> const &imageToSubstract) : Minus(1, imageToSubstract) {}

    void operator()(std::shared_ptr<PairTile<T>> tiles) override {
        auto t1 = tiles->first;
        auto t2 = tiles->second;

        for (size_t y = 0; y < t1->height(); ++y) {
            for (size_t x = 0; x < t1->width(); ++x) {
                Pixel<T> p1 = t1->at(x, y);
                Pixel<T> p2 = t2->at(x, y);
                t1->set(x, y, p1 - p2);
            }
        }
        this->addResult(t1);
    }

    auto createTiggerState() {
        auto triggerState =
            std::make_shared<TriggerState<AbstractImage<T>, AbstractImage<T>>>(
                imageToSubstract_);
        return std::make_shared<
            hh::StateManager<1, AbstractImage<T>, AbstractImage<T>>>(
            triggerState);
    }

    std::shared_ptr<typename Abstract2TilesAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<Minus<T>>(this->numberThreads(),
                                          imageToSubstract_);
    }

    constexpr static auto setup(std::shared_ptr<HedgehogPipeline<T>> pipeline,
                                auto self) {
        auto minusWrapper = std::make_shared<MinusTileWrapper<T>>(1);
        auto minusState = std::make_shared<MinusState<T>>();
        auto minusStateManager =
            std::make_shared<MinusStateManager<T>>(minusState);
        auto triggerStateManager = self->createTiggerState();

        if constexpr (null_type_v<F> && null_type_v<L>) {
            auto splitGraph = pipeline->createSplitGraph();
            pipeline->inputs(triggerStateManager);
            pipeline->edges(triggerStateManager, splitGraph);
            pipeline->edges(splitGraph, minusWrapper);
        } else {
            auto splitGraph =
                pipeline->createSplitGraph(self->pipeline_->ghostRegionSize());

            self->pipeline_->inputs(triggerStateManager);
            self->pipeline_->edges(triggerStateManager, splitGraph);
            self->pipeline_->edges(splitGraph, self->pipeline_->firstTask());
            self->pipeline_->outputs(self->pipeline_->lastTask());

            pipeline->inputs(self->pipeline_->graph());
            pipeline->edges(self->pipeline_->graph(), minusWrapper);
        }
        pipeline->edges(minusWrapper, minusStateManager);
        return pipeline->add(minusStateManager)->add(self);
    }

  private:
    Image<T> imageToSubstract_ = nullptr;
    std::shared_ptr<HedgehogPipeline<T, F, L>> pipeline_ = nullptr;
};

}; // namespace hhimg::hdg

#endif
