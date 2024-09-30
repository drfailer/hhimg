#ifndef CONVOLUTION_HPP
#define CONVOLUTION_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_image_factory.h"
#include "../abstract/hh/abstract_pair_tile_algorithm.h"
#include "../concrete/data/mask.h"
#include "../tools/perf_recorder.h"
#include "hhimg/algorithm/tile/tmp_tiles_graph.h"
#include "hhimg/algorithm/tile/update_stencils_graph.h"

namespace hhimg {

template <typename T, typename MaskType>
class Convolution : public AbstractAlgorithm<T>,
                    public AbstractPairTileAlgorithm<T> {
  public:
    Convolution(size_t nbThreads, Mask<MaskType> const &kernel,
                MaskType bias = 0)
        : AbstractPairTileAlgorithm<T>(nbThreads, "Convolution"),
          kernel_(kernel), bias_(bias) {}
    Convolution(std::shared_ptr<AbstractImageFactory<T>> imageFactory,
                Mask<MaskType> const &kernel, MaskType bias = 0)
        : Convolution(1, kernel, bias) {
        imageFactory_ = imageFactory;
    }
    ~Convolution() = default;

    Image<T> operator()(Image<T> image) const override {
        utils::PerfRectorder::start("Convolute");
        size_t halfWidth = kernel_.width() / 2;
        size_t halfHeight = kernel_.height() / 2;
        size_t beginX = halfWidth, endX = image->width() - halfWidth;
        size_t beginY = halfHeight, endY = image->height() - halfHeight;
        auto result =
            imageFactory_->get(image->width() - kernel_.width() + 1,
                               image->height() - kernel_.height() + 1);

        for (size_t y = beginY; y < endY; ++y) {
            for (size_t x = beginX; x < endX; ++x) {
                auto value = computeValue(x, y, image);
                size_t rx = x - beginX;
                size_t ry = y - beginY;
                result->set(rx, ry, pixelValidCast<T>(value + bias_));
            }
        }
        utils::PerfRectorder::end("Convolute");
        return result;
    }

    void operator()(std::shared_ptr<PairTile<T>> tiles) override {
        auto tile = tiles->first;
        auto origin = tiles->second;

        for (size_t y = 0; y < tile->height(); ++y) {
            for (size_t x = 0; x < tile->width(); ++x) {
                auto value = computeValue(x, y, origin);
                tile->set(x, y, pixelValidCast<T>(value + bias_));
            }
        }
        this->addResult(tile);
    }

    size_t ghostRegionSize() const override {
        return std::max(kernel_.width() / 2, kernel_.height() / 2);
    }

    std::shared_ptr<typename AbstractPairTileAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<Convolution<T, MaskType>>(this->numberThreads(),
                                                          kernel_, bias_);
    }

    constexpr static auto setup(auto pipeline, auto self) {
        auto updateStencils = std::make_shared<hhimg::UpdateStencilsGraph<T>>();
        auto pairGraph = std::make_shared<hhimg::TmpTilesGraph<T>>(self);
        return pipeline->add(updateStencils)->add(pairGraph);
    }

  private:
    std::shared_ptr<AbstractImageFactory<T>> imageFactory_ = nullptr;
    Mask<MaskType> kernel_;
    MaskType bias_ = 0;

    Pixel<MaskType> computeValue(size_t x, size_t y, auto elt) const {
        int halfWidth = kernel_.width() / 2;
        int halfHeight = kernel_.height() / 2;
        Pixel<MaskType> result = {0, 0, 0};

        for (size_t my = 0; my < kernel_.height(); ++my) {
            for (size_t mx = 0; mx < kernel_.width(); ++mx) {
                int ix = (int)x + (int)mx - halfWidth;
                int iy = (int)y + (int)my - halfHeight;
                result +=
                    pixelCast<MaskType>(elt->at(ix, iy)) * kernel_.at(mx, my);
            }
        }
        return result;
    }
};

} // namespace hhimg

#endif
