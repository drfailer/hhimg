#include "impl/cimg/cimg_image.h"
#include <iostream>
#include <CImg/CImg.h>

using PixelType = unsigned char;

int main(int, char**) {
    /* cimg_library::CImg<PixelType> image("../img/test-rgb.png"); */
    CImgImage<PixelType> image("../img/test-rgb.png");
    cimg_library::CImg<PixelType> visu(500,400,1,3,0);
    const PixelType red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };

    std::cout << image.width() << "x" << image.height() << std::endl;
    std::cout << image.size() << std::endl;

    unsigned char p;
    p= image.image().at(image.width());
    std::cout << (int) p << std::endl;
    p = image.image().at(image.width() + (image.width() / 2));
    std::cout << (int) p << std::endl;
    p = image.image().at(image.width() + image.width());
    std::cout << (int) p << std::endl;
    std::cout << "dept: " << image.image().depth() << std::endl;

    for (size_t i = 0; i < image.height(); ++i) {
        image.at(image.width() / 2, i)->set(255, 255, 255);
    }

    /* cimg_library::CImgDisplay main_disp(image.crop(0, 0, 0, 0, image.width() - 1, image.height() - 1, 0, 1),"Click a point"); */
    cimg_library::CImgDisplay main_disp(image.image(),"Click a point");
    cimg_library::CImgDisplay draw_disp(visu,"Intensity profile");


    while (!main_disp.is_closed() && !draw_disp.is_closed()) {
        main_disp.wait();
        if (main_disp.button() && main_disp.mouse_y()>=0) {
            const int y = main_disp.mouse_y();
            visu.fill(0)
                .draw_graph(image.image().get_crop(0,y,0,0,image.width()-1,y,0,0),red,1,1,0,255,0)
                .draw_graph(image.image().get_crop(0,y,0,1,image.width()-1,y,0,1),green,1,1,0,255,0)
                .draw_graph(image.image().get_crop(0,y,0,2,image.width()-1,y,0,2),blue,1,1,0,255,0)
                .display(draw_disp);
        }
    }
    return 0;
}
