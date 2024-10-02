#ifndef ABSTRACT_IMAGE_FACTORY_HPP
#define ABSTRACT_IMAGE_FACTORY_HPP
#include "../../common/abstract_image.h"
#include <cstddef>
#include <memory>

namespace hhimg::seq {

template <typename T> class AbstractImageFactory {
  public:
    AbstractImageFactory() = default;
    virtual ~AbstractImageFactory() = default;

    virtual std::shared_ptr<AbstractImage<T>> get(size_t width,
                                                  size_t height) const = 0;
};

} // namespace hhimg::seq

#endif
