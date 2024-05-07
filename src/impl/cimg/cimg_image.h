#ifndef CIMG_IMAGE_HPP
#define CIMG_IMAGE_HPP
#include <hhimg/hhimg.h>
#include <CImg/CImg.h>

template <typename T> class CImgImage : public hhimg::AbstractImage<T> {
  public:
    CImgImage(std::string const &filename)
        : hhimg::AbstractImage<unsigned char>(filename),
          image_(filename.c_str()) {}
    CImgImage(CImgImage const &other)
        : hhimg::AbstractImage<unsigned char>(other.filename()),
          image_(other.image_) {}
    CImgImage(size_t width, size_t height)
        : hhimg::AbstractImage<unsigned char>(""),
          image_(width, height) {}

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

    T &red(size_t offset) override { return image_.at(offset); }
    T &green(size_t offset) override {
        return image_.at(offset + width() * height() * image_.depth());
    }
    T &blue(size_t offset) override {
        return image_.at(offset + 2 * width() * height() * image_.depth());
    }
    T red(size_t offset) const override { return image_.at(offset); }
    T green(size_t offset) const override {
        return image_.at(offset + width() * height() * image_.depth());
    }
    T blue(size_t offset) const override {
        return image_.at(offset + 2 * width() * height() * image_.depth());
    }

  private:
    cimg_library::CImg<T> image_;
};

#endif
