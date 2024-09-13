#include "cimg.h"
#include "safe_cimg.h"

template <typename PixelType>
void displayCImgImage(std::shared_ptr<CImgImage<PixelType>> image) {
    cimg_library::CImgDisplay main_disp(image->image(), "Click a point");

    while (!main_disp.is_closed()) {
        main_disp.wait();
    }
}

// force template generation
template void displayCImgImage<PixelType>(std::shared_ptr<CImgImage<PixelType>>);
