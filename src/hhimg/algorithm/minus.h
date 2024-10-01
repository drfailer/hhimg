#ifndef MINUS_HPP
#define MINUS_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/hh/tile_algorithms.h"
#include "../algorithm/tile/minus/minus_state.h"
#include "../algorithm/tile/minus/minus_state_manager.h"
#include "../algorithm/tile/minus/minus_wrapper.h"
#include "../concrete/data/fl_img.h"
#include "../concrete/hedgehog_pipeline.h"
#include "../tools/perf_recorder.h"
#include "hhimg/algorithm/tile/trigger_state.h"

namespace hhimg {

template <typename T>
class Minus : public AbstractAlgorithm<T>, public Abstract2TilesAlgorithm<T> {
  public:
    Minus(size_t nbThreads, Image<T> const &imageToSubstract)
        : Abstract2TilesAlgorithm<T>(nbThreads, "Minus"),
          imageToSubstract_(imageToSubstract) {}
    Minus(Image<T> const &imageToSubstract) : Minus(1, imageToSubstract) {}
    Minus(FLImg<T> const &flimg) : flImageToSubstract_(flimg) {}
    ~Minus() = default;

    Image<T> operator()(Image<T> image) const override {
        utils::PerfRectorder::start("Minus");
        if (image->width() != imageToSubstract_->width() ||
            image->height() != imageToSubstract_->height()) {
            throw std::invalid_argument(
                "error: can't substract images with different shapes");
        }
        for (size_t y = 0; y < image->height(); ++y) {
            for (size_t x = 0; x < image->width(); ++x) {
                Pixel<T> p1 = image->at(x, y);
                Pixel<T> p2 = imageToSubstract_->at(x, y);
                image->set(x, y, p1 - p2);
            }
        }
        utils::PerfRectorder::end("Minus");
        return image;
    }

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

    // todo: setup for the fast loader pipeline

  private:
    Image<T> imageToSubstract_ = nullptr;
    FLImg<T> flImageToSubstract_;
};

}; // namespace hhimg

#endif
