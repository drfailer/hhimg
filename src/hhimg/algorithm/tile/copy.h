#ifndef HHIMG_COPY_H
#define HHIMG_COPY_H
#include "../../abstract/abstract_tile_factory.h"
#include "../../abstract/hh/tile_algorithms.h"

namespace hhimg {

template <typename T> struct Copy : AbstractTileAlgorithm<T> {
    Copy(size_t nbThreads, std::shared_ptr<AbstractTileFactory<T>> tileFactory)
        : AbstractTileAlgorithm<T>(nbThreads, "Copy"),
          tileFactory(tileFactory) {}

    void operator()(std::shared_ptr<AbstractTile<T>> tile) override {
        tileFactory->copy(tile);
        this->addResult(tile);
    }

    std::shared_ptr<typename AbstractTileAlgorithm<T>::TaskType>
    copy() override {
        return std::make_shared<Copy>(this->numberThreads(), tileFactory);
    }

    std::shared_ptr<AbstractTileFactory<T>> tileFactory = nullptr;
};

} // end namespace hhimg

#endif
