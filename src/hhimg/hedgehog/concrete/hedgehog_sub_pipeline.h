#ifndef HEDGEHOG_SUB_PIPELINE_H
#define HEDGEHOG_SUB_PIPELINE_H
#include "hedgehog_pipeline.h"
#include <hedgehog/hedgehog.h>

namespace hhimg::hdg {

template <typename T, typename FirstTask = NullType,
          typename LastTask = NullType>
struct HedgehogSubPipeline : private HedgehogPipeline<T, FirstTask, LastTask> {
    HedgehogSubPipeline(Image<T> image)
        : HedgehogPipeline<T, FirstTask, LastTask>(0, 0, 0, nullptr, ""),
          image_(image) {}

  private:
    Image<T> image_ = nullptr;
};

} // namespace hhimg::hdg

#endif
