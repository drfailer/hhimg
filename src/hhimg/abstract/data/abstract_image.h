#ifndef ABSTRACT_IMAGE_HPP
#define ABSTRACT_IMAGE_HPP
#include "../../concrete/data/pixel.h"
#include <cstddef>
#include <memory>
#include <string>
#include <iostream>

namespace hhimg {

template <typename T> class AbstractImage {
  public:
    AbstractImage(std::string const &filename) : filename_(filename) {}
    AbstractImage(AbstractImage const &other)
        : AbstractImage(other.filename()) {}
    AbstractImage(AbstractImage &&other) : AbstractImage(other.filename()) {}
    virtual ~AbstractImage() {}

    // image size
    virtual size_t width() const = 0;
    virtual size_t height() const = 0;

    virtual void load(std::string const &filename) = 0;
    virtual void save(std::string const &filename) = 0;

    virtual std::shared_ptr<AbstractImage<T>> copy() const = 0;
    virtual void set(std::shared_ptr<AbstractImage<T>> &&other) = 0;

    // access to the image
    virtual Pixel<T> at(size_t offset) const = 0;
    virtual void set(size_t offset, Pixel<T> const &pixel) = 0;

    // acces with x,y coordinates (can be overrided for optimization)
    virtual Pixel<T> at(size_t x, size_t y) const { return at(y * width() + x); }
    virtual void set(size_t x, size_t y, Pixel<T> const &pixel) {
        set(y * width() + x, pixel);
    }

    // set without pixel
    virtual void set(size_t offset, T r, T g, T b) {
        set(offset, {r, g, b});
    }
    virtual void set(size_t x, size_t y, T r, T g, T b) {
        set(y * width() + x, r, g, b);
    }

    // for grayscaled images
    void set(size_t offset, T v) { set(offset, v, v, v); }
    void set(size_t x, size_t y, T v) { set(x, y, v, v, v); }
    T get(size_t offset) const { return at(offset).red; }
    T get(size_t x, size_t y) const { return at(x, y).red; }

    size_t size() const { return width() * height(); }
    const std::string &filename() const { return filename_; }
    void filename(std::string const &filename) { filename_ = filename; }

    // type
    using type = T;

  private:
    std::string filename_ = "";
};

} // namespace hhimg

#endif
