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
  src/hhimg/algorithm/convolution.h
  src/hhimg/algorithm/tile/minus/minus_state.h
  src/hhimg/algorithm/tile/minus/minus_state_manager.h
  src/hhimg/algorithm/tile/minus/minus_wrapper.h
  src/hhimg/algorithm/tile/split_graph/split_state.h
  src/hhimg/algorithm/tile/split_graph/split_graph.h
  src/hhimg/algorithm/tile/split_graph/split_task.h
  src/hhimg/algorithm/tile/trigger_state.h
  src/hhimg/algorithm/tile/update_stencils_state.h
  src/hhimg/algorithm/tile/copy.h
  src/hhimg/algorithm/tile/create_tmp_tiles.h
  src/hhimg/algorithm/tile/update_stencils_task.h
  src/hhimg/algorithm/tile/tmp_tiles_graph.h
  src/hhimg/algorithm/tile/split.h
  src/hhimg/algorithm/tile/update_stencils_state_manager.h
  src/hhimg/algorithm/tile/update_stencils_graph.h
  src/hhimg/algorithm/map_mutate.h
  src/hhimg/algorithm/threshold.h
  src/hhimg/algorithm/crop.h
  src/hhimg/algorithm/gray_scale.h
  src/hhimg/algorithm/rgb_map.h
  src/hhimg/algorithm/contrast_brightness.h
  src/hhimg/algorithm/fast_loader/test_algorithm.h
  src/hhimg/algorithm/minus.h
  src/hhimg/concrete/data/fl_img.h
  src/hhimg/concrete/data/pixel_tile.h
  src/hhimg/concrete/data/mask.h
  src/hhimg/concrete/data/fl_view.h
  src/hhimg/concrete/data/pixel.h
  src/hhimg/concrete/hedgehog_pipeline.h
  src/hhimg/concrete/chain_algorithm.h
  src/hhimg/concrete/fast_loader_pipeline.h
  src/hhimg/tools/null_type.h
  src/hhimg/tools/concepts.h
  src/hhimg/tools/perf_recorder.h
  src/hhimg/tools/algorithm_operators.h
  src/hhimg/tools/utils.h
  src/hhimg/hhimg.h
  src/hhimg/abstract/abstract_tile_factory.h
  src/hhimg/abstract/hh/tile_algorithms.h
  src/hhimg/abstract/hh/abstract_hh_algorithm.h
  src/hhimg/abstract/data/abstract_tile.h
  src/hhimg/abstract/data/abstract_pixel_container.h
  src/hhimg/abstract/data/abstract_image.h
  src/hhimg/abstract/abstract_image_factory.h
  src/hhimg/abstract/abstract_algorithm.h
  src/hhimg/abstract/fast_loader/abstract_fl_algorithm.h
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
