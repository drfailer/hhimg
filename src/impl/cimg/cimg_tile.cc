#include "cimg_tile.h"
#include "safe_cimg.h"

template <typename T>
CImgTile<T>::CImgTile(size_t x, size_t y, size_t tileSize,
                      std::shared_ptr<CImgImage<T>> image)
    : hhimg::AbstractTile<T>(x, y, tileSize, image),
      dataRed_(&image->image()._data[y * image->width() + x]),
      dataBlue_(&image->image()._data[y * image->width() + x +
                                      image->width() * image->height() *
                                          image->image().depth()]),
      dataGreen_(&image->image()._data[y * image->width() + x +
                                       2 * image->width() * image->height() *
                                           image->image().depth()]),
      image_(image) {}

template <typename T> hhimg::Pixel<T> CImgTile<T>::at(size_t offset) const {
    return {dataRed_[offset], dataGreen_[offset], dataBlue_[offset]};
}

template <typename T>
void CImgTile<T>::set(size_t offset, hhimg::Pixel<T> const &pixel) {
    dataRed_[offset] = pixel.red;
    dataGreen_[offset] = pixel.green;
    dataBlue_[offset] = pixel.blue;
}

template <typename T>
std::shared_ptr<hhimg::AbstractImage<T>> CImgTile<T>::image() const {
    return image_;
}

// force template generation
template class CImgTile<PixelType>;
