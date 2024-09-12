#include <hedgehog/hedgehog.h>
#include "config.h"
#include "hhimg/algorithm/contrast_brightness.h"
#include "impl/cimg/cimg.h"
#include "impl/cimg/cimg_tile_factory.h"
#include <hhimg/hhimg.h>

using PixelType = unsigned char;

void verticalBordersExtr(std::shared_ptr<CImgImage<PixelType>> image) {
    hhimg::Mask<double> verticalBorders({-1, 0, 1}, 3, 1);
    auto imageFactory = std::make_shared<CImgImageFactory<PixelType>>();

    hhimg::utils::PerfRectorder::start("verticalBordersExtr");
    image |=
        hhimg::GrayScale<PixelType>() |
        hhimg::Convolution<PixelType, double>(imageFactory, verticalBorders) |
        hhimg::NonMaximumSuppression<PixelType>(50);
    hhimg::utils::PerfRectorder::end("verticalBordersExtr");
}

void horizontalBordersExtr(std::shared_ptr<CImgImage<PixelType>> image) {
    hhimg::Mask<double> horizontalBorders({-1, 0, 1}, 1, 3);
    auto imageFactory = std::make_shared<CImgImageFactory<PixelType>>();

    hhimg::utils::PerfRectorder::start("horizontalBordersExtr");
    image |=
        hhimg::GrayScale<PixelType>() |
        hhimg::Convolution<PixelType, double>(imageFactory, horizontalBorders) |
        hhimg::NonMaximumSuppression<PixelType>(50);
    hhimg::utils::PerfRectorder::end("horizontalBordersExtr");
}

void detailExtr(std::shared_ptr<CImgImage<PixelType>> image) {
    auto secondImage = image->copy();
    std::vector<double> v(9, 1.0 / 9);
    hhimg::Mask<double> meanFilter(v, 3, 3);
    auto imageFactory = std::make_shared<CImgImageFactory<PixelType>>();

    hhimg::utils::PerfRectorder::start("detailExtr");
    secondImage |=
        hhimg::Convolution<PixelType, double>(imageFactory, meanFilter);
    image |= hhimg::Crop<PixelType>(imageFactory, 1, 1, secondImage->width(),
                                    secondImage->height()) |
             hhimg::Minus<PixelType>(secondImage) |
             hhimg::GrayScale<PixelType>() |
             hhimg::NonMaximumSuppression<PixelType>(10);
    hhimg::utils::PerfRectorder::end("detailExtr");
}

void generateRainbow() {
    auto imageFactory = std::make_shared<CImgImageFactory<PixelType>>();
    auto computeRed = [](auto, size_t x, size_t) { return 255 - x; };
    auto computeGreen = [](auto, size_t, size_t y) { return y; };
    auto computeBlue = [](auto, size_t x, size_t) { return x; };
    auto image = imageFactory->get(255, 255);

    image |=
        hhimg::RGBMapMutate<PixelType>(computeRed, computeGreen, computeBlue);
    displayCImgImage(std::static_pointer_cast<CImgImage<PixelType>>(image));
}

void redFilter(std::shared_ptr<hhimg::AbstractImage<PixelType>> image) {
    auto imageFactory = std::make_shared<CImgImageFactory<PixelType>>();
    auto computeRed = [image](auto, size_t x, size_t y) {
        return image->at(x, y).red;
    };
    auto computeGreen = [image](auto, size_t, size_t) { return 0; };
    auto computeBlue = [image](auto, size_t, size_t) { return 0; };

    image |=
        hhimg::RGBMapMutate<PixelType>(computeRed, computeGreen, computeBlue);
}

void contrast(std::shared_ptr<hhimg::AbstractImage<PixelType>> image) {
  /* image |= hhimg::ContrastBrightness<PixelType>(0.5); */
  image |= hhimg::ContrastBrightness<PixelType>(1.5);
  /* image |= hhimg::ContrastBrightness<PixelType>(1); */
}

void run(Config config) {
    hhimg::utils::PerfRectorder::start("Image load");
    auto image = std::make_shared<CImgImage<PixelType>>(config.filename);
    hhimg::utils::PerfRectorder::end("Image load");
    auto tileFactory = std::make_shared<CImgTileFactory<PixelType>>();

    image |= std::make_shared<hhimg::Split<PixelType>>(256, tileFactory) |
             std::static_pointer_cast<hhimg::AbstractTileAlgorithm<PixelType>>(
                 std::make_shared<hhimg::GrayScale<PixelType>>()) |
             std::static_pointer_cast<hhimg::AbstractTileAlgorithm<PixelType>>(
                 std::make_shared<hhimg::NonMaximumSuppression<PixelType>>(50));

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

    std::cout << "image size: " << image->width() << "x" << image->height()
              << std::endl;
    if (config.display) {
        displayCImgImage(image);
    }
}

int main(int argc, char **argv) {
    Config config;

    parseCmdArgs(argc, argv, config);
    run(config);
    hhimg::utils::PerfRectorder::report();
    return 0;
}
