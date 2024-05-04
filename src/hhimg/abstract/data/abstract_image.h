#ifndef ABSTRACT_IMAGE_HPP
#define ABSTRACT_IMAGE_HPP
/* #include "../abstract_algorithm.h" */
#include "abstract_pixel.h"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

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
    size_t size() const { return width() * height(); }

    virtual void load(std::string const &filename) = 0;
    virtual void save(std::string const &filename) = 0;

    virtual std::shared_ptr<AbstractImage<T>> copy() const = 0;
    virtual void set(std::shared_ptr<AbstractImage<T>> &&other) = 0;

    const std::string &filename() const { return filename_; }
    void filename(std::string const &filename) { filename_ = filename; }

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

  protected:
    virtual std::shared_ptr<AbstractPixel<T>> atImpl(size_t offset) = 0;

  private:
    std::string filename_ = "";
};

} // namespace hhimg

// operator-=
template <typename Img>
std::shared_ptr<Img> const &operator-=(std::shared_ptr<Img> &lhs,
                                       std::shared_ptr<Img> const &rhs) {
    if (lhs->width() != rhs->width() || lhs->height() != rhs->height()) {
        throw std::invalid_argument(
            "error: can't substract images with different shapes");
    }
    for (size_t y = 0; y < lhs->height(); ++y) {
        for (size_t x = 0; x < lhs->width(); ++x) {
            lhs->at(x, y) -= rhs->at(x, y);
        }
    }
    return lhs;
}

// operator-
template <typename Img>
std::shared_ptr<Img> const &operator-(std::shared_ptr<Img> const &lhs,
                                      std::shared_ptr<Img> const &rhs) {
    if (lhs->width() != rhs->width() || lhs->height() != rhs->height()) {
        throw std::invalid_argument(
            "error: can't substract images with different shapes");
    }
    auto result = lhs->copy();
    return result -= rhs;
}

#endif
