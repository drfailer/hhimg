#include "impl/cimg/cimg_image.h"
#include <CImg/CImg.h>
#include <hhimg/hhimg.h>
#include "config.h"

using PixelType = unsigned char;
#undef GrayScale

void displayCImgImage(std::shared_ptr<CImgImage<PixelType>> image) {
    cimg_library::CImgDisplay main_disp(image->image(), "Click a point");

    while (!main_disp.is_closed()) {
        main_disp.wait();
    }
}

void verticalBordersExtr(std::shared_ptr<CImgImage<PixelType>> image) {
    hhimg::Mask<double> verticalBorders({-1, 0, 1}, 3, 1);

    hhimg::utils::PerfRectorder::start("verticalBordersExtr");
    image |= hhimg::GrayScale<PixelType>() |
             hhimg::Convolute<PixelType, double>(verticalBorders) |
             hhimg::NonMaximumSuppression<PixelType>(50);
    hhimg::utils::PerfRectorder::end("verticalBordersExtr");
}

void horizontalBordersExtr(std::shared_ptr<CImgImage<PixelType>> image) {
    hhimg::Mask<double> horizontalBorders({-1, 0, 1}, 1, 3);

    hhimg::utils::PerfRectorder::start("horizontalBordersExtr");
    image |= hhimg::GrayScale<PixelType>() |
             hhimg::Convolute<PixelType, double>(horizontalBorders) |
             hhimg::NonMaximumSuppression<PixelType>(50);
    hhimg::utils::PerfRectorder::end("horizontalBordersExtr");
}

void detailExtr(std::shared_ptr<CImgImage<PixelType>> image) {
    auto secondImage = image->copy();
    std::vector<double> v(9, 1.0 / 9);
    hhimg::Mask<double> meanFilter(v, 3, 3);

    hhimg::utils::PerfRectorder::start("detailExtr");
    secondImage |= hhimg::Convolute<PixelType, double>(meanFilter);
    image |= hhimg::Minus<PixelType>(secondImage) |
             hhimg::NonMaximumSuppression<PixelType>(10);
    hhimg::utils::PerfRectorder::end("detailExtr");
}

void run(Config config) {
    hhimg::utils::PerfRectorder::start("Image load");
    auto image = std::make_shared<CImgImage<PixelType>>(config.filename);
    hhimg::utils::PerfRectorder::end("Image load");

    switch (config.algorithm) {
    case VerticalBorders:
        verticalBordersExtr(image);
        break;
    case HorizontalBorders:
        horizontalBordersExtr(image);
        break;
    default:
        detailExtr(image);
        break;
    }

    std::cout << "image size: " << image->width() << "x" << image->height() <<
        std::endl;
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
