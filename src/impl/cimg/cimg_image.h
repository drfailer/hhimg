#ifndef CIMG_IMAGE_HPP
#define CIMG_IMAGE_HPP
#include <CImg/CImg.h>
#include <hhimg/hhimg.h>

template <typename T> class CImgImage : public hhimg::AbstractImage<T> {
  public:
    CImgImage(std::string const &filename)
        : hhimg::AbstractImage<unsigned char>(filename),
          image_(filename.c_str()) {}
    CImgImage(CImgImage const &other)
        : hhimg::AbstractImage<unsigned char>(other.filename()),
          image_(other.image_) {}
    CImgImage(size_t width, size_t height)
        : hhimg::AbstractImage<unsigned char>(""), image_(width, height, 1, 3) {}

    const cimg_library::CImg<T> &image() const { return image_; }
    cimg_library::CImg<T> &image() { return image_; }

    size_t width() const override { return image_.width(); }
    size_t height() const override { return image_.height(); }

    void load(std::string const &filename) override {
        this->filename(filename);
        image_.load(filename.c_str());
    }

    void save(std::string const &filename) override {
        image_.save(filename.c_str());
    }

    std::shared_ptr<hhimg::AbstractImage<T>> copy() const override {
        return std::make_shared<CImgImage<T>>(*this);
    }

    void set(std::shared_ptr<hhimg::AbstractImage<T>> &&other) override {
        auto i = std::static_pointer_cast<CImgImage<T>>(other);
        this->filename(other->filename());
        image_.swap(i->image());
    }

    hhimg::Pixel<T> at(size_t offset) const override {
        return {image_.at(offset),
                image_.at(offset + width() * height() * image_.depth()),
                image_.at(offset + 2 * width() * height() * image_.depth())};
    }

    void set(size_t offset, hhimg::Pixel<T> const &pixel) override {
        image_.at(offset) = pixel.red;
        image_.at(offset + width() * height() * image_.depth()) = pixel.green;
        image_.at(offset + 2 * width() * height() * image_.depth()) =
            pixel.blue;
    }

    /* hhimg::Pixel<T> at(size_t x, size_t y) const override { */
    /*     return {image_.atXY(x, y, 0, 0), image_.atXY(x, y, 0, 1), image_.atXY(x, y, 0, 2)}; */
    /* } */

    /* void set(size_t x, size_t y, hhimg::Pixel<T> const &pixel) override { */
    /*     image_.atXY(x, y, 0, 0) = pixel.red; */
    /*     image_.atXY(x, y, 0, 1) = pixel.green; */
    /*     image_.atXY(x, y, 0, 2) = pixel.blue; */
    /* } */

  private:
    cimg_library::CImg<T> image_;
};

#endif
