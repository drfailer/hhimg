#include "cimg_tile_factory.h"
#include "safe_cimg.h"

template <typename T>
std::shared_ptr<hhimg::PixelTile<T>>
CImgTileFactory<T>::get(size_t x, size_t y, size_t ghostRegionSize) const {
    auto tile = std::make_shared<hhimg::PixelTile<T>>(x, y, this->tileSize_,
                                                      ghostRegionSize, image_);
    auto dataRed =
        &image_->image().atXYZC(tile->ghostX(), tile->ghostY(), 0, 0);
    auto dataGreen =
        &image_->image().atXYZC(tile->ghostX(), tile->ghostY(), 0, 1);
    auto dataBlue =
        &image_->image().atXYZC(tile->ghostX(), tile->ghostY(), 0, 2);

    for (size_t iy = 0; iy < tile->ghostHeight(); ++iy) {
        for (size_t ix = 0; ix < tile->ghostWidth(); ++ix) {
            tile->ghostSet(ix, iy,
                           hhimg::Pixel<T>{
                               dataRed[ix + iy * image_->fullWidth()],
                               dataGreen[ix + iy * image_->fullWidth()],
                               dataBlue[ix + iy * image_->fullWidth()],
                           });
        }
    }
    return tile;
}

template struct CImgTileFactory<PixelType>;
