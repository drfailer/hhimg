#ifndef ABSTRACT_IMAGE_HPP
#define ABSTRACT_IMAGE_HPP
/* #include "../abstract_algorithm.h" */
#include "abstract_pixel.h"
#include <cstddef>
#include <memory>
#include <string>

namespace hhimg {

template <typename T> class AbstractImage {
  public:
    AbstractImage(std::string const &filename) : filename_(filename) {}
    AbstractImage(AbstractImage const &other)
        : AbstractImage(other.filename()) {}
    AbstractImage(AbstractImage &&other) : AbstractImage(other.filename()) {}
    virtual ~AbstractImage() {}

    const std::string &filename() const { return filename_; }
    void filename(std::string const &filename) { filename_ = filename; }

    // image size
    virtual size_t width() const = 0;
    virtual size_t height() const = 0;
    size_t size() const { return width() * height(); }

    // get pixels
    std::shared_ptr<AbstractPixel<T>> at(size_t offset) {
        return atImpl(offset);
    }

    std::shared_ptr<AbstractPixel<T>> at(int x, int y) {
        return atImpl(y * width() + x);
    };

    // get pixels const
    const std::shared_ptr<AbstractPixel<T>> at(size_t offset) const {
        return atImpl(offset);
    }

    const std::shared_ptr<AbstractPixel<T>> at(int x, int y) const {
        return atImpl(y * width() + x);
    }

    // operator[]
    std::shared_ptr<AbstractPixel<T>> operator[](size_t offset) {
        return atImpl(offset);
    }

    const std::shared_ptr<AbstractPixel<T>> operator[](size_t offset) const {
        return atImpl(offset);
    }

    virtual void load(std::string const &filename) = 0;
    virtual void save(std::string const &filename) = 0;

    virtual std::shared_ptr<AbstractImage<T>> copy() const = 0;
    virtual void set(std::shared_ptr<AbstractImage<T>> &&other) = 0;

  protected:
    virtual std::shared_ptr<AbstractPixel<T>> atImpl(size_t offset) = 0;

  private:
    std::string filename_ = "";
};

} // namespace hhimg

#endif
