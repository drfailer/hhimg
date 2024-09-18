#include "cimg_tile.h"
#include "safe_cimg.h"

template <typename T> size_t imageSize(std::shared_ptr<CImgImage<T>> image) {
    return image->width() * image->height() * image->image().depth();
}

template <typename T>
CImgTile<T>::CImgTile(size_t x, size_t y, size_t tileSize,
                      size_t ghostRegionSize,
                      std::shared_ptr<CImgImage<T>> image)
    : hhimg::AbstractTile<T>(x, y, tileSize, ghostRegionSize, image) {
    size_t offset = this->ghostY() * this->fullWidth() + this->ghostX();
    size_t size = imageSize(image);
    dataRed_ = &image->image()._data[offset];
    dataGreen_ = &image->image()._data[offset + size];
    dataBlue_ = &image->image()._data[offset + 2 * size];
}

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
hhimg::Pixel<T> CImgTile<T>::ghostAt(size_t offset) const {
    return {dataRed_[offset], dataGreen_[offset], dataBlue_[offset]};
}

template <typename T>
void CImgTile<T>::ghostSet(size_t offset, hhimg::Pixel<T> const &pixel) {
    dataRed_[offset] = pixel.red;
    dataGreen_[offset] = pixel.green;
    dataBlue_[offset] = pixel.blue;
}

// force template generation
template class CImgTile<PixelType>;
