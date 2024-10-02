#ifndef HDG_MINUS_HPP
#define HDG_MINUS_HPP
#include "../../tools/hhutils/trigger_state.h"
#include "../abstract/tile_algorithms.h"
#include "../concrete/hedgehog_pipeline.h"
#include "tile/minus/minus_state.h"
#include "tile/minus/minus_state_manager.h"
#include "tile/minus/minus_wrapper.h"

namespace hhimg::hdg {

template <typename T> class Minus : public Abstract2TilesAlgorithm<T> {
  public:
    Minus(size_t nbThreads, Image<T> const &imageToSubstract)
        : Abstract2TilesAlgorithm<T>(nbThreads, "Minus"),
          imageToSubstract_(imageToSubstract) {}
    Minus(Image<T> const &imageToSubstract) : Minus(1, imageToSubstract) {}
    ~Minus() = default;

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
        return std::make_shared<
            TriggerState<AbstractImage<T>, AbstractImage<T>>>(
            imageToSubstract_);
    }

    std::shared_ptr<typename Abstract2TilesAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<Minus<T>>(this->numberThreads(),
                                          imageToSubstract_);
    }

    constexpr static auto setup(std::shared_ptr<HedgehogPipeline<T>> pipeline,
                                auto self) {
        auto triggerState = self->createTiggerState();
        auto triggerStateManager = std::make_shared<
            hh::StateManager<1, AbstractImage<T>, AbstractImage<T>>>(
            triggerState);
        auto splitGraph = pipeline->createSplitGraph();
        auto minusWrapper = std::make_shared<MinusTileWrapper<T>>(1);
        auto minusState = std::make_shared<MinusState<T>>();
        auto minusStateManager =
            std::make_shared<MinusStateManager<T>>(minusState);

        pipeline->inputs(triggerStateManager);
        pipeline->edges(triggerStateManager, splitGraph);
        pipeline->edges(splitGraph, minusWrapper);
        pipeline->edges(minusWrapper, minusStateManager);
        return pipeline->add(minusStateManager)->add(self);
    }

  private:
    Image<T> imageToSubstract_ = nullptr;
};

}; // namespace hhimg::hdg

#endif
