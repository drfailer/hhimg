#ifndef HHIMG_HPP
#define HHIMG_HPP


#include "./concrete/chain_algorithm.h"
#include "./concrete/hedgehog_pipeline.h"
#include "./concrete/data/mask.h"
#include "./concrete/fast_loader_pipeline.h"
#include "./concrete/data/fl_img.h"
#include "./concrete/data/fl_view.h"
#include "./algorithm/gray_scale.h"
#include "./algorithm/non_maximum_suppression.h"
#include "./algorithm/crop.h"
#include "./algorithm/minus.h"
#include "./algorithm/convolution.h"
#include "./algorithm/rgb_map.h"
#include "./algorithm/map_mutate.h"
#include "./tools/perf_recorder.h"
#include "./tools/algorithm_operators.h"
#include "./tools/utils.h"
#include "./abstract/abstract_algorithm.h"
#include "./abstract/hh/abstract_hh_algorithm.h"
#include "./abstract/hh/tile_algorithms.h"
#include "./abstract/abstract_image_factory.h"
#include "./abstract/abstract_tile_factory.h"
#include "./abstract/data/abstract_image.h"

#endif
