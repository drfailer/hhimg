#ifndef CONVOLUTION_HPP
#define CONVOLUTION_HPP
#include "../abstract/abstract_algorithm.h"
#include "../abstract/abstract_image_factory.h"
#include "../abstract/abstract_pair_tile_algorithm.h"
#include "../abstract/abstract_tile_algorithm.h"
#include "../concrete/data/mask.h"
#include "../tools/perf_recorder.h"

namespace hhimg {

template <typename T, typename MaskType>
class Convolution : public AbstractAlgorithm<T>,
                    public AbstractPairTileAlgorithm<T> {
  public:
    Convolution(size_t nbThreads, Mask<MaskType> const &kernel,
                MaskType bias = 0)
        : AbstractPairTileAlgorithm<T>("Convolution", nbThreads),
          kernel_(kernel), bias_(bias) {}
    Convolution(std::shared_ptr<AbstractImageFactory<T>> imageFactory,
                Mask<MaskType> const &kernel, MaskType bias = 0)
        : Convolution(0, kernel, bias) {
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

    void
    operator()(std::shared_ptr<std::pair<Tile<T>, Tile<T>>> tiles) override {
        auto tile = tiles->first;
        auto ghost = tiles->second;

        // apply the mask
        for (size_t y = 0; y < tile->height(); ++y) {
            for (size_t x = 0; x < tile->width(); ++x) {
                auto value = computeValue(x, y, ghost);
                tile->set(x, y, pixelValidCast<T>(value + bias_));
            }
        }
        this->addResult(tile);
    }

    size_t ghostRegionSize() const override {
        return std::max(kernel_.width() / 2, kernel_.height() / 2);
    }

    std::shared_ptr<PairTaskType<T>> copy() override {
        return std::make_shared<Convolution<T, MaskType>>(this->numberThreads(),
                                                          kernel_, bias_);
    }

  private:
    std::shared_ptr<AbstractImageFactory<T>> imageFactory_ = nullptr;
    Mask<MaskType> kernel_;
    MaskType bias_ = 0;

    Pixel<MaskType> computeValue(size_t x, size_t y, Image<T> image) const {
        int halfWidth = kernel_.width() / 2;
        int halfHeight = kernel_.height() / 2;
        Pixel<MaskType> result = {0, 0, 0};

        for (size_t my = 0; my < kernel_.height(); ++my) {
            for (size_t mx = 0; mx < kernel_.width(); ++mx) {
                size_t ix = x + mx - halfWidth;
                size_t iy = y + my - halfHeight;
                result +=
                    pixelCast<MaskType>(image->at(ix, iy)) * kernel_.at(mx, my);
            }
        }
        return result;
    }

    Pixel<MaskType> computeValue(size_t x, size_t y, Tile<T> tile) const {
        int halfWidth = kernel_.width() / 2;
        int halfHeight = kernel_.height() / 2;
        Pixel<MaskType> result = {0, 0, 0};

        for (size_t my = 0; my < kernel_.height(); ++my) {
            for (size_t mx = 0; mx < kernel_.width(); ++mx) {
                size_t ix = x + mx - halfWidth + tile->ghostRegionSize();
                size_t iy = y + my - halfHeight + tile->ghostRegionSize();
                if (ix < tile->ghostWidth() && iy < tile->ghostWidth()) {
                    result += pixelCast<MaskType>(tile->ghostAt(ix, iy)) *
                              kernel_.at(mx, my);
                }
            }
        }
        return result;
    }
};

} // namespace hhimg

#endif
