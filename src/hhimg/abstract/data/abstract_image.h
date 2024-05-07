#ifndef ABSTRACT_IMAGE_HPP
#define ABSTRACT_IMAGE_HPP
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

    // image size
    virtual size_t width() const = 0;
    virtual size_t height() const = 0;

    virtual void load(std::string const &filename) = 0;
    virtual void save(std::string const &filename) = 0;

    virtual std::shared_ptr<AbstractImage<T>> copy() const = 0;
    virtual void set(std::shared_ptr<AbstractImage<T>> &&other) = 0;

    // note: no alpha for now
    virtual T &red(size_t offset) = 0;
    virtual T &green(size_t offset) = 0;
    virtual T &blue(size_t offset) = 0;
    virtual T red(size_t offset) const = 0;
    virtual T green(size_t offset) const = 0;
    virtual T blue(size_t offset) const = 0;

    T &red(size_t x, size_t y) { return red(y * width() + x); }
    T &green(size_t x, size_t y) { return green(y * width() + x); }
    T &blue(size_t x, size_t y) { return blue(y * width() + x); }
    T red(size_t x, size_t y) const { return red(y * width() + x); }
    T green(size_t x, size_t y) const { return green(y * width() + x); }
    T blue(size_t x, size_t y) const { return blue(y * width() + x); }

    // virtual as it can be optimized
    virtual void set(size_t offset, T r, T g, T b) {
        red(offset) = r;
        green(offset) = g;
        blue(offset) = b;
    }
    void set(size_t offset, T v) { set(offset, v, v, v); }
    void set(size_t x, size_t y, T r, T g, T b) {
        set(y * width() + x, r, g, b);
    }
    void set(size_t x, size_t y, T v) { set(y * width() + x, v); }

    // for grayscaled image
    T get(size_t x, size_t y) const { return red(x, y); }
    T get(size_t offset) const { return red(offset); }

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
