#include "cimg_image.h"
#include "safe_cimg.h"

template <typename T>
CImgImage<T>::CImgImage(std::string const &filename)
    : hhimg::AbstractImage<unsigned char>(filename),
      image_(std::make_shared<cimg_library::CImg<T>>(filename.c_str())) {}

template <typename T>
CImgImage<T>::CImgImage(CImgImage const &other)
    : hhimg::AbstractImage<unsigned char>(other.filename()),
      image_(std::make_shared<cimg_library::CImg<T>>(*other.image_)) {}

template <typename T>
CImgImage<T>::CImgImage(size_t width, size_t height)
    : hhimg::AbstractImage<unsigned char>(""),
      image_(std::make_shared<cimg_library::CImg<T>>(width, height, 1, 3)) {}

template <typename T> const cimg_library::CImg<T> &CImgImage<T>::image() const {
    return *image_;
}

template <typename T> cimg_library::CImg<T> &CImgImage<T>::image() {
    return *image_;
}

template <typename T> size_t CImgImage<T>::width() const {
    return image_->width();
}

template <typename T> size_t CImgImage<T>::height() const {
    return image_->height();
}

template <typename T> void CImgImage<T>::load(std::string const &filename) {
    this->filename(filename);
    image_->load(filename.c_str());
}

template <typename T> void CImgImage<T>::save(std::string const &filename) {
    image_->save(filename.c_str());
}

template <typename T> hhimg::Pixel<T> CImgImage<T>::at(size_t offset) const {
    return {image_->at(offset),
            image_->at(offset + width() * height() * image_->depth()),
            image_->at(offset + 2 * width() * height() * image_->depth())};
}

template <typename T>
void CImgImage<T>::set(std::shared_ptr<hhimg::AbstractImage<T>> &&other) {
    auto i = std::dynamic_pointer_cast<CImgImage<T>>(other);
    this->filename(other->filename());
    image_->swap(i->image());
}

template <typename T>
void CImgImage<T>::set(size_t offset, hhimg::Pixel<T> const &pixel) {
    image_->at(offset) = pixel.red;
    image_->at(offset + width() * height() * image_->depth()) = pixel.green;
    image_->at(offset + 2 * width() * height() * image_->depth()) = pixel.blue;
}

template <typename T>
std::shared_ptr<hhimg::AbstractImage<T>> CImgImage<T>::copy() const {
    return std::make_shared<CImgImage<T>>(*this);
}

// force template generation
template class CImgImage<PixelType>;
