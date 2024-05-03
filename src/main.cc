#include "impl/cimg/cimg_image.h"
#include <CImg/CImg.h>
/* #include <hhimg/hhimg.h> */

using PixelType = unsigned char;
#undef GrayScale

int main(int, char**) {
    /* cimg_library::CImg<PixelType> image("../img/test-rgb.png"); */
    CImgImage<PixelType> image("../img/rivendell.jpg");

    image |= hhimg::GrayScale<PixelType>();

    /* cimg_library::CImgDisplay main_disp(image.crop(0, 0, 0, 0, image.width() - 1, image.height() - 1, 0, 1),"Click a point"); */
    cimg_library::CImgDisplay main_disp(image.image(),"Click a point");

    while (!main_disp.is_closed()) {
        main_disp.wait();
    }
    return 0;
}
