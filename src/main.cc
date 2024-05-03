#include "hhimg/concrete/data/mask.h"
#include "impl/cimg/cimg_image.h"
#include <CImg/CImg.h>
/* #include <hhimg/hhimg.h> */

using PixelType = unsigned char;
#undef GrayScale

int main(int, char**) {
    auto image = std::make_shared<CImgImage<PixelType>>("../img/rivendell.jpg");
    std::vector<double> v(9, 1.0/9);
    hhimg::Mask<double> meanFilter(v, 3, 3);
    hhimg::Mask<double> horizontalBorders({-1, 0, 1}, 3, 1);

    image |= hhimg::GrayScale<PixelType>() | hhimg::MaskApplier<PixelType>(horizontalBorders);

    /* cimg_library::CImgDisplay main_disp(image.crop(0, 0, 0, 0, image.width() - 1, image.height() - 1, 0, 1),"Click a point"); */
    cimg_library::CImgDisplay main_disp(image->image(),"Click a point");

    while (!main_disp.is_closed()) {
        main_disp.wait();
    }
    return 0;
}
