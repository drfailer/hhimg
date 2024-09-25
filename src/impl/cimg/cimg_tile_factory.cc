#include "cimg_tile_factory.h"
#include "impl/cimg/cimg_image.h"
#include "safe_cimg.h"

template <typename T>
std::shared_ptr<hhimg::PixelTile<T>>
CImgTileFactory<T>::get(size_t x, size_t y, size_t tileSize,
                        size_t ghostRegionSize,
                        std::shared_ptr<hhimg::AbstractImage<T>> image) const {
    auto cimgImage = std::dynamic_pointer_cast<CImgImage<T>>(image);
    auto tile = std::make_shared<hhimg::PixelTile<T>>(x, y, tileSize,
                                                      ghostRegionSize, image);
    auto dataRed = &cimgImage->image().atXYZC(tile->x(), tile->y(), 0, 0);
    auto dataGreen = &cimgImage->image().atXYZC(tile->x(), tile->y(), 0, 1);
    auto dataBlue = &cimgImage->image().atXYZC(tile->x(), tile->y(), 0, 2);

    for (size_t y = 0; y < tile->height(); ++y) {
        for (size_t x = 0; x < tile->width(); ++x) {
            tile->set(x, y,
                      hhimg::Pixel<T>{
                          dataRed[x + y * image->fullWidth()],
                          dataGreen[x + y * image->fullWidth()],
                          dataBlue[x + y * image->fullWidth()],
                      });
        }
    }
    return tile;
}

template <typename T>
void CImgTileFactory<T>::copy(
    std::shared_ptr<hhimg::AbstractTile<T>> tile) const {
    auto cimgImage = std::dynamic_pointer_cast<CImgImage<T>>(tile->image());
    T *dataRed = &cimgImage->image().atXYZC(tile->x(), tile->y(), 0, 0);
    T *dataGreen = &cimgImage->image().atXYZC(tile->x(), tile->y(), 0, 1);
    T *dataBlue = &cimgImage->image().atXYZC(tile->x(), tile->y(), 0, 2);

    for (size_t y = 0; y < tile->height(); ++y) {
        for (size_t x = 0; x < tile->width(); ++x) {
            auto pixel = tile->at(x, y);
            dataRed[y * cimgImage->fullWidth() + x] = pixel.red;
            dataGreen[y * cimgImage->fullWidth() + x] = pixel.green;
            dataBlue[y * cimgImage->fullWidth() + x] = pixel.blue;
        }
    }
}

template struct CImgTileFactory<PixelType>;
