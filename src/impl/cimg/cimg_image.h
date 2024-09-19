#ifndef CIMG_IMAGE_HPP
#define CIMG_IMAGE_HPP
#include "hhimg/abstract/data/abstract_image.h"

namespace cimg_library {
template <typename T> class CImg;
}

template <typename T> class CImgImage : public hhimg::AbstractImage<T> {
  public:
    CImgImage(std::string const &filename);
    CImgImage(CImgImage const &other);
    CImgImage(size_t width, size_t height);

    const cimg_library::CImg<T> &image() const;
    cimg_library::CImg<T> &image();

    size_t width() const override;
    size_t height() const override;
    size_t fullWidth() const override;

    void load(std::string const &filename) override;
    void save(std::string const &filename) override;

    hhimg::Pixel<T> at(size_t offset) const override;
    void set(std::shared_ptr<hhimg::AbstractImage<T>> &&other) override;
    void set(size_t offset, hhimg::Pixel<T> const &pixel) override;

    std::shared_ptr<hhimg::AbstractImage<T>> copy() const override;

    using hhimg::AbstractImage<T>::at;
    using hhimg::AbstractImage<T>::set;

  private:
    std::shared_ptr<cimg_library::CImg<T>> image_;
};

#endif
