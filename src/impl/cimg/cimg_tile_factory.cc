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
    auto dataRed =
        &cimgImage->image().atXYZC(tile->ghostX(), tile->ghostY(), 0, 0);
    auto dataGreen =
        &cimgImage->image().atXYZC(tile->ghostX(), tile->ghostY(), 0, 1);
    auto dataBlue =
        &cimgImage->image().atXYZC(tile->ghostX(), tile->ghostY(), 0, 2);
    size_t tyBegin = tile->ghostY() == 0 ? tile->ghostRegionSize() : 0;
    size_t txBegin = tile->ghostX() == 0 ? tile->ghostRegionSize() : 0;

    for (size_t ty = tyBegin, iy = 0; ty < tile->ghostHeight(); ty++, ++iy) {
        for (size_t tx = txBegin, ix = 0; tx < tile->ghostWidth(); tx++, ++ix) {
            tile->ghostSet(tx, ty,
                           hhimg::Pixel<T>{
                               dataRed[ix + iy * image->fullWidth()],
                               dataGreen[ix + iy * image->fullWidth()],
                               dataBlue[ix + iy * image->fullWidth()],
                           });
        }
    }
    return tile;
}

template struct CImgTileFactory<PixelType>;
