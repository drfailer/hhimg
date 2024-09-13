#ifndef CIMG_H
#define CIMG_H

#include "./cimg_image.h"
#include "./cimg_image_factory.h"

template <typename PixelType>
void displayCImgImage(std::shared_ptr<CImgImage<PixelType>> image);

#endif
