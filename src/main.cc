#include "config.h"
#include "hhimg/algorithm/contrast_brightness.h"
#include "hhimg/algorithm/fast_loader/test_algorithm.h"
#include "hhimg/concrete/fast_loader_pipeline.h"
#include "impl/cimg/cimg.h"
#include "impl/cimg/cimg_tile_factory.h"
#include "impl/tiff_fl/grayscale_tiff_tile_loader.h"
#include <hedgehog/hedgehog.h>
#include <hhimg/hhimg.h>

using PixelType = unsigned char;

void verticalBordersExtr(std::shared_ptr<CImgImage<PixelType>> image) {
    hhimg::Mask<double> verticalBorders({-1, 0, 1}, 3, 1);
    auto imageFactory = std::make_shared<CImgImageFactory<PixelType>>();

    hhimg::utils::PerfRectorder::start("verticalBordersExtr");
    image |=
        hhimg::GrayScale<PixelType>() |
        hhimg::Convolution<PixelType, double>(imageFactory, verticalBorders) |
        hhimg::Threshold<PixelType>(50);
    hhimg::utils::PerfRectorder::end("verticalBordersExtr");
}

void horizontalBordersExtr(std::shared_ptr<CImgImage<PixelType>> image) {
    hhimg::Mask<double> horizontalBorders({-1, 0, 1}, 1, 3);
    auto imageFactory = std::make_shared<CImgImageFactory<PixelType>>();

    hhimg::utils::PerfRectorder::start("horizontalBordersExtr");
    image |=
        hhimg::GrayScale<PixelType>() |
        hhimg::Convolution<PixelType, double>(imageFactory, horizontalBorders) |
        hhimg::Threshold<PixelType>(50);
    hhimg::utils::PerfRectorder::end("horizontalBordersExtr");
}

void detailExtr(std::shared_ptr<CImgImage<PixelType>> image) {
    hhimg::Mask<double> meanFilter(std::vector<double>(9, 1.0 / 9), 3, 3);
    auto secondImage = image->copy();
    auto imageFactory = std::make_shared<CImgImageFactory<PixelType>>();

    hhimg::utils::PerfRectorder::start("detailExtr");
    secondImage |=
        hhimg::Convolution<PixelType, double>(imageFactory, meanFilter);
    image |= hhimg::Crop<PixelType>(imageFactory, 1, 1, secondImage->width(),
                                    secondImage->height()) |
             hhimg::Minus<PixelType>(secondImage) |
             hhimg::GrayScale<PixelType>() |
             hhimg::Threshold<PixelType>(10);
    hhimg::utils::PerfRectorder::end("detailExtr");
}

void detailExtrHH(std::shared_ptr<CImgImage<PixelType>> image) {
    hhimg::Mask<double> meanFilter(std::vector<double>(9, 1.0 / 9), 3, 3);
    auto tileFactory = std::make_shared<CImgTileFactory<PixelType>>();
    auto secondImage = image->copy();

    hhimg::utils::PerfRectorder::start("detailExtr");
    secondImage |=
        std::make_shared<hhimg::HedgehogPipeline<PixelType>>(
            256, 4, 4, tileFactory, "mean") |
        std::make_shared<hhimg::Convolution<PixelType, double>>(32, meanFilter);
    image |= std::make_shared<hhimg::HedgehogPipeline<PixelType>>(
                 256, 4, 4, tileFactory, "border extraction") |
             std::make_shared<hhimg::Minus<PixelType>>(10, secondImage) |
             std::make_shared<hhimg::GrayScale<PixelType>>(10) |
             std::make_shared<hhimg::Threshold<PixelType>>(10, 10);
    hhimg::utils::PerfRectorder::end("detailExtr");
}

void generateRainbow() {
    auto imageFactory = std::make_shared<CImgImageFactory<PixelType>>();
    auto compute = [](auto, size_t x, size_t y) {
        return hhimg::Pixel<PixelType>{PixelType(255 - x), (PixelType)y,
                                       (PixelType)x};
    };
    auto image = imageFactory->get(255, 255);

    image |= hhimg::MapMutate<PixelType>(compute);
    displayCImgImage(std::static_pointer_cast<CImgImage<PixelType>>(image));
}

void redFilter(std::shared_ptr<hhimg::AbstractImage<PixelType>> image) {
    auto imageFactory = std::make_shared<CImgImageFactory<PixelType>>();
    auto compute = [](auto image, size_t x, size_t y) {
        return hhimg::Pixel<PixelType>{image->at(x, y).red, 0, 0};
    };

    image |= hhimg::MapMutate<PixelType>(compute);
}

void contrast(std::shared_ptr<hhimg::AbstractImage<PixelType>> image) {
    image |= hhimg::ContrastBrightness<PixelType>(1.5, 1);
}

void testHedgehog(std::shared_ptr<hhimg::AbstractImage<PixelType>> image) {
    auto tileFactory = std::make_shared<CImgTileFactory<PixelType>>();
    std::vector<double> v(9, 1.0 / 9);
    /* std::vector<double> v(9, 2.0); */
    hhimg::Mask<double> meanFilter(v, 3, 3);
    auto compute = [](auto tile, size_t x, size_t y) {
        return hhimg::Pixel<PixelType>{tile->at(x, y).red, 0,
                                       tile->at(x, y).blue};
    };

    image |= std::make_shared<hhimg::HedgehogPipeline<PixelType>>(
                 256, 4, 4, tileFactory, "my algorithm") |
             std::make_shared<hhimg::Convolution<PixelType, double>>(
                 32, meanFilter) |
             std::make_shared<hhimg::MapMutate<PixelType>>(32, compute);
}

void halideBlur(std::shared_ptr<hhimg::AbstractImage<PixelType>> image) {
    auto tileFactory = std::make_shared<CImgTileFactory<PixelType>>();
    hhimg::Mask<double> blurX({1. / 3., 1. / 3., 1. / 3.}, 3, 1);
    hhimg::Mask<double> blurY({1. / 3., 1. / 3., 1. / 3.}, 1, 3);
    /* std::vector<double> v(9, 1.0 / 9); */
    /* hhimg::Mask<double> meanFilter(v, 3, 3); */

    image |=
        std::make_shared<hhimg::HedgehogPipeline<PixelType>>(
            256, 8, 8, tileFactory, "Halide blur") |
        std::make_shared<hhimg::Convolution<PixelType, double>>(20, blurX) |
        std::make_shared<hhimg::Convolution<PixelType, double>>(20, blurY);
    /* image |= */
    /*     std::make_shared<hhimg::HedgehogPipeline<PixelType>>(256, tileFactory, */
    /*                                                          "Halide blur") | */
    /*     std::static_pointer_cast<hhimg::AbstractPairTileAlgorithm<PixelType>>( */
    /*         std::make_shared<hhimg::Convolution<PixelType, double>>(32, */
    /*           meanFilter)); */
}

void run(Config config) {
    hhimg::utils::PerfRectorder::start("Image load");
    auto image = std::make_shared<CImgImage<PixelType>>(config.filename);
    hhimg::utils::PerfRectorder::end("Image load");

    hhimg::utils::PerfRectorder::start("run");

    /* hhimg::FLImg<PixelType> flimg = { */
    /*   std::make_shared<GrayscaleTiffTileLoader<PixelType>>(1, "../img/img_r022_c026_c1.ome.tif") */
    /* }; */

    /* flimg |= */
    /*     std::make_shared<hhimg::FastLoaderPipeline<PixelType>>(256, "test") | */
    /*     std::make_shared<hhimg::TestAlgorithm<PixelType>>(1); */

    /* testHedgehog(image); */
    detailExtrHH(image);
    /* halideBlur(image); */

    /* switch (config.algorithm) { */
    /* case VerticalBorders: */
    /*     verticalBordersExtr(image); */
    /*     break; */
    /* case HorizontalBorders: */
    /*     horizontalBordersExtr(image); */
    /*     break; */
    /* case Detail: */
    /*     detailExtr(image); */
    /*     break; */
    /* case Rainbow: */
    /*     generateRainbow(); */
    /*     break; */
    /* case RedFilter: */
    /*     redFilter(image); */
    /*     break; */
    /* case Contrast: */
    /*     contrast(image); */
    /*     break; */
    /* } */

    hhimg::utils::PerfRectorder::end("run");

    std::cout << "image size: " << image->width() << "x" << image->height()
              << std::endl;
    if (config.display) {
        displayCImgImage(image);
    }
    // save image for test
    hhimg::utils::PerfRectorder::start("Image save");
    image->save("../img/save.jpg");
    hhimg::utils::PerfRectorder::end("Image save");
}

int main(int argc, char **argv) {
    Config config;

    parseCmdArgs(argc, argv, config);
    run(config);
    hhimg::utils::PerfRectorder::report();
    return 0;
}
