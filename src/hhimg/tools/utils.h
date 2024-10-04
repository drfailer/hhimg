#ifndef UTILS_HPP
#define UTILS_HPP
#include <numeric>
#include <vector>
#include <cstddef>
#include <functional>

namespace hhimg::utils {

template <typename T> T validate(T value) {
    if (value < 0)
        return 0;
    if (value > 255)
        return 255;
    return value;
}

template <typename T> T computeSize(std::vector<T> const &sizes) {
    return std::accumulate(sizes.begin(), sizes.end(), 1,
                           std::multiplies<size_t>());
}

} // namespace hhimg::utils

#endif
