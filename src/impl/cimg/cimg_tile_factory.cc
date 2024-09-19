#include "cimg_tile_factory.h"
#include "safe_cimg.h"

template <typename T>
std::shared_ptr<hhimg::PixelTile<T>>
CImgTileFactory<T>::get(size_t x, size_t y, size_t ghostRegionSize) const {
    auto tile = std::make_shared<hhimg::PixelTile<T>>(x, y, this->tileSize_,
                                                      ghostRegionSize, image_);
    size_t offset = tile->ghostY() * image_->width() + tile->ghostX();
    size_t size = image_->width() * image_->height() * image_->image().depth();
    auto dataRed = &image_->image()._data[offset];
    auto dataGreen = &image_->image()._data[offset + size];
    auto dataBlue = &image_->image()._data[offset + 2 * size];

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
