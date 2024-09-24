#ifndef HHIMG_COPY_H
#define HHIMG_COPY_H
#include "../../abstract/hh/abstract_tile_algorithm.h"
#include "../../../impl/cimg/safe_cimg.h"
#include "impl/cimg/cimg_image.h"

namespace hhimg {

template <typename T> struct Copy : AbstractTileAlgorithm<T> {
    Copy(size_t nbThreads) : AbstractTileAlgorithm<T>(nbThreads, "Copy") {}

    void operator()(std::shared_ptr<AbstractTile<T>> tile) override {
        auto cimg = std::dynamic_pointer_cast<CImgImage<T>>(tile->image());
        T *r = &cimg->image().atXYZC(tile->x(), tile->y(), 0, 0);
        T *g = &cimg->image().atXYZC(tile->x(), tile->y(), 0, 1);
        T *b = &cimg->image().atXYZC(tile->x(), tile->y(), 0, 2);

        for (size_t y = 0; y < tile->height(); ++y) {
            for (size_t x = 0; x < tile->width(); ++x) {
                auto pixel = tile->at(x, y);
                r[y * cimg->fullWidth() + x] = pixel.red;
                g[y * cimg->fullWidth() + x] = pixel.green;
                b[y * cimg->fullWidth() + x] = pixel.blue;
            }
        }
        this->addResult(tile);
    }

    std::shared_ptr<typename AbstractTileAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<Copy>(this->numberThreads());
    }
};

} // end namespace hhimg

#endif
