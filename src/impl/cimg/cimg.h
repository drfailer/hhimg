#ifndef CIMG_H
#define CIMG_H

#include "./cimg_image.h"
#include "./cimg_image_factory.h"

template <typename PixelType>
void displayCImgImage(std::shared_ptr<CImgImage<PixelType>> image) {
    cimg_library::CImgDisplay main_disp(image->image(), "Click a point");

    while (!main_disp.is_closed()) {
        main_disp.wait();
    }
}

#endif
