#ifndef HDG_CONVOLUTION_HPP
#define HDG_CONVOLUTION_HPP
#include "../abstract/tile_algorithms.h"
#include "../../common/mask.h"
#include "tile/tmp_tiles_graph.h"
#include "tile/update_borders/update_stencils_graph.h"

namespace hhimg::hdg {

template <typename T, typename MaskType>
class Convolution : public AbstractTmpTileAlgorithm<T> {
  public:
    Convolution(size_t nbThreads, Mask<MaskType> const &kernel,
                MaskType bias = 0)
        : AbstractTmpTileAlgorithm<T>(nbThreads, "Convolution"),
          kernel_(kernel), bias_(bias) {}
    Convolution(Mask<MaskType> const &kernel, MaskType bias = 0)
      : Convolution(1, kernel, bias) {}


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

    std::shared_ptr<typename AbstractTmpTileAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<Convolution<T, MaskType>>(this->numberThreads(),
                                                          kernel_, bias_);
    }

    constexpr static auto setup(auto pipeline, auto self) {
        auto updateStencils = std::make_shared<UpdateStencilsGraph<T>>();
        auto pairGraph = std::make_shared<TmpTilesGraph<T>>(self);
        return pipeline->add(updateStencils)->add(pairGraph);
    }

  private:
    Mask<MaskType> kernel_;
    MaskType bias_ = 0;

    // TODO: should be common
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

} // namespace hhimg::hdg

#endif
