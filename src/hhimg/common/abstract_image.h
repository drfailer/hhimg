#ifndef ABSTRACT_IMAGE_HPP
#define ABSTRACT_IMAGE_HPP
#include "abstract_pixel_container.h"
#include <memory>
#include <string>

namespace hhimg {

template <typename T> class AbstractImage : public AbstractPixelContainer<T> {
  public:
    AbstractImage(std::string const &filename) : filename_(filename) {}
    AbstractImage(AbstractImage const &other)
        : AbstractImage(other.filename()) {}
    AbstractImage(AbstractImage &&other) : AbstractImage(other.filename()) {}
    virtual ~AbstractImage() {}

    virtual void load(std::string const &filename) = 0;
    virtual void save(std::string const &filename) = 0;

    virtual std::shared_ptr<AbstractImage<T>> copy() const = 0;
    virtual void set(std::shared_ptr<AbstractImage<T>> &&other) = 0;

    const std::string &filename() const { return filename_; }
    void filename(std::string const &filename) { filename_ = filename; }

    using AbstractPixelContainer<T>::set;
    using AbstractPixelContainer<T>::at;

  private:
    std::string filename_ = "";
};

template <typename T> using Image = std::shared_ptr<AbstractImage<T>>;

} // namespace hhimg

#endif
