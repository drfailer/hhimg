cmake_minimum_required(VERSION 3.16)
project(hh-img)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(TEST_HHIMG OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules"
  "${CMAKE_CURRENT_SOURCE_DIR}/lib/FastLoader/cmake_modules"
)

set(Hedgehog_INCLUDE_DIR ./lib/hedgehog/)
set(FastLoader_INCLUDE_DIR ./lib/FastLoader/)

# added tclap
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/tiff-4.7.0/lib/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/src/)
link_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/tiff-4.7.0/lib/lib/)

# Need to defined variable Hedgehog_INCLUDE_DIR to the hedgehog path
# hedgehog
find_package(Hedgehog REQUIRED)
find_package(X11 QUIET)
find_package(FastLoader REQUIRED)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${Hedgehog_CXX_FLAGS})

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Werror -pedantic -O3")
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fsanitize=address -fno-omit-frame-pointer -Wextra -Werror -Wuninitialized -pedantic -g")
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wextra -Werror -Wuninitialized -pedantic -g")
endif (NOT MSVC)

# files of the project
set(hh_img_files
  src/config.h
  src/hhimg/sequential/algorithm/convolution.h
  src/hhimg/sequential/algorithm/map_mutate.h
  src/hhimg/sequential/algorithm/threshold.h
  src/hhimg/sequential/algorithm/crop.h
  src/hhimg/sequential/algorithm/gray_scale.h
  src/hhimg/sequential/algorithm/rgb_map.h
  src/hhimg/sequential/algorithm/contrast_brightness.h
  src/hhimg/sequential/algorithm/minus.h
  src/hhimg/sequential/concrete/data/pixel.h
  src/hhimg/sequential/concrete/chain_algorithm.h
  src/hhimg/sequential/abstract/data/abstract_pixel_container.h
  src/hhimg/sequential/abstract/abstract_image_factory.h
  src/hhimg/sequential/abstract/abstract_algorithm.h
  src/hhimg/sequential/operators.h
  src/hhimg/hedgehog/algorithm/convolution.h
  src/hhimg/hedgehog/algorithm/tile/minus/minus_state.h
  src/hhimg/hedgehog/algorithm/tile/minus/minus_state_manager.h
  src/hhimg/hedgehog/algorithm/tile/minus/minus_wrapper.h
  src/hhimg/hedgehog/algorithm/tile/split_graph/split_state.h
  src/hhimg/hedgehog/algorithm/tile/split_graph/split_graph.h
  src/hhimg/hedgehog/algorithm/tile/split_graph/split_task.h
  src/hhimg/hedgehog/algorithm/tile/update_borders/update_stencils_state.h
  src/hhimg/hedgehog/algorithm/tile/update_borders/update_stencils_task.h
  src/hhimg/hedgehog/algorithm/tile/update_borders/update_stencils_state_manager.h
  src/hhimg/hedgehog/algorithm/tile/update_borders/update_stencils_graph.h
  src/hhimg/hedgehog/algorithm/tile/copy.h
  src/hhimg/hedgehog/algorithm/tile/create_tmp_tiles.h
  src/hhimg/hedgehog/algorithm/tile/tmp_tiles_graph.h
  src/hhimg/hedgehog/algorithm/map_mutate.h
  src/hhimg/hedgehog/algorithm/threshold.h
  src/hhimg/hedgehog/algorithm/gray_scale.h
  src/hhimg/hedgehog/algorithm/contrast_brightness.h
  src/hhimg/hedgehog/algorithm/minus.h
  src/hhimg/hedgehog/helpers.h
  src/hhimg/hedgehog/concrete/data/pixel_tile.h
  src/hhimg/hedgehog/concrete/hedgehog_sub_pipeline.h
  src/hhimg/hedgehog/concrete/hedgehog_pipeline.h
  src/hhimg/hedgehog/abstract/tile_algorithms.h
  src/hhimg/hedgehog/abstract/abstract_tile_factory.h
  src/hhimg/hedgehog/abstract/data/abstract_tile.h
  src/hhimg/hedgehog/abstract/abstract_hh_algorithm.h
  src/hhimg/hedgehog/operators.h
  src/hhimg/macros.h
  src/hhimg/tools/null_type.h
  src/hhimg/tools/concepts.h
  src/hhimg/tools/perf_recorder.h
  src/hhimg/tools/utils.h
  src/hhimg/tools/hhutils/trigger_state.h
  src/hhimg/tools/hhutils/identity_state.h
  src/hhimg/common/mask.h
  src/hhimg/common/abstract_pixel_container.h
  src/hhimg/common/abstract_image.h
  src/hhimg/common/pixel.h
  src/hhimg/hhimg.h
  src/hhimg/fast_loader/algorithm/convolution.h
  src/hhimg/fast_loader/algorithm/map_mutate.h
  src/hhimg/fast_loader/algorithm/threshold.h
  src/hhimg/fast_loader/algorithm/gray_scale.h
  src/hhimg/fast_loader/algorithm/views/minus/minus_state.h
  src/hhimg/fast_loader/algorithm/views/minus/minus_state_manager.h
  src/hhimg/fast_loader/algorithm/views/minus/minus_wrapper.h
  src/hhimg/fast_loader/algorithm/views/split_graph/split_state.h
  src/hhimg/fast_loader/algorithm/views/split_graph/split_graph.h
  src/hhimg/fast_loader/algorithm/views/split_graph/split_task.h
  src/hhimg/fast_loader/algorithm/views/update_borders/update_stencils_state.h
  src/hhimg/fast_loader/algorithm/views/update_borders/update_stencils_task.h
  src/hhimg/fast_loader/algorithm/views/update_borders/update_stencils_state_manager.h
  src/hhimg/fast_loader/algorithm/views/update_borders/update_stencils_graph.h
  src/hhimg/fast_loader/algorithm/views/copy.h
  src/hhimg/fast_loader/algorithm/views/create_tmp_tiles.h
  src/hhimg/fast_loader/algorithm/views/tmp_tiles_graph.h
  src/hhimg/fast_loader/algorithm/views/release_view.h
  src/hhimg/fast_loader/algorithm/rgb_map.h
  src/hhimg/fast_loader/algorithm/contrast_brightness.h
  src/hhimg/fast_loader/algorithm/fast_loader/test_algorithm.h
  src/hhimg/fast_loader/algorithm/minus.h
  src/hhimg/fast_loader/concrete/data/fl_img.h
  src/hhimg/fast_loader/concrete/data/fl_view.h
  src/hhimg/fast_loader/concrete/fast_loader_pipeline.h
  src/hhimg/fast_loader/abstract/view_algorithms.h
  src/hhimg/fast_loader/abstract/abstract_fl_algorithm.h
  src/hhimg/fast_loader/operators.h
  src/impl/cimg/cimg_image.h
  src/impl/cimg/cimg.h
  src/impl/cimg/safe_cimg.h
  src/impl/cimg/cimg_image_factory.h
  src/impl/cimg/cimg_tile_factory.h
  src/impl/tiff_fl/grayscale_tiff_tile_loader.h
)

# # Gtest
include(FetchContent) # Used to download GTEST
if (TEST_HHIMG)
    message("Fetch Gtest")
    if (POLICY CMP0135)
        FetchContent_Declare(
                googletest
                URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
                DOWNLOAD_EXTRACT_TIMESTAMP true
        )
    else ()
        FetchContent_Declare(
                googletest
                URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        )
    endif ()

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(hhimg_test tests/hhimg_test.cc ${hh_img_files})
    target_link_libraries(hhimg_test gtest_main)

    include(GoogleTest)
    gtest_discover_tests(hhimg_test)
endif (TEST_HHIMG)

add_library(cimg SHARED
  src/impl/cimg/cimg.cc
  src/impl/cimg/cimg_image.cc
  src/impl/cimg/cimg_tile_factory.cc
)

# main executable
add_executable(
  hh-img src/main.cc
  ${hh_img_files})
target_link_libraries(hh-img
  PRIVATE cimg tiff ${X11_LIBRARIES} ${FastLoader_LIBRARIES} ${Hedgehog_LIBRARIES})


# target for dependencies
add_custom_target(depts
  COMMAND ./build.sh
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)
