#include "impl/cimg/cimg_image.h"
#include <CImg/CImg.h>
#include <hhimg/hhimg.h>

using PixelType = unsigned char;
#undef GrayScale

void displayCImgImage(std::shared_ptr<CImgImage<PixelType>> image) {
    cimg_library::CImgDisplay main_disp(image->image(), "Click a point");

    while (!main_disp.is_closed()) {
        main_disp.wait();
    }
}

void verticalBordersExtr(std::string const &filename) {
    auto image = std::make_shared<CImgImage<PixelType>>(filename);
    hhimg::Mask<double> verticalBorders({-1, 0, 1}, 3, 1);

    image |= hhimg::GrayScale<PixelType>() |
             hhimg::Convolute<PixelType, double>(verticalBorders) |
             hhimg::NonMaximumSuppression<PixelType>(50);
    displayCImgImage(image);
}

void horizontalBordersExtr(std::string const &filename) {
    auto image = std::make_shared<CImgImage<PixelType>>(filename);
    hhimg::Mask<double> horizontalBorders({-1, 0, 1}, 1, 3);

    image |= hhimg::GrayScale<PixelType>() |
             hhimg::Convolute<PixelType, double>(horizontalBorders) |
             hhimg::NonMaximumSuppression<PixelType>(50);
    displayCImgImage(image);
}

void detailExtr(std::string const &filename) {
    auto image = std::make_shared<CImgImage<PixelType>>(filename);
    auto secondImage = image->copy();
    std::vector<double> v(9, 1.0 / 9);
    hhimg::Mask<double> meanFilter(v, 3, 3);

    secondImage |= hhimg::Convolute<PixelType, double>(meanFilter);
    image |= hhimg::Minus<PixelType>(secondImage) |
             hhimg::NonMaximumSuppression<PixelType>(10);
    displayCImgImage(image);
}

int main(int, char **) {
    /* verticalBordersExtr("../img/rivendell.jpg"); */
    /* horizontalBordersExtr("../img/rivendell.jpg"); */
    detailExtr("../img/rivendell.jpg");
    return 0;
}
