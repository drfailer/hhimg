cmake_minimum_required(VERSION 3.16)
project(hh-img)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(TEST_HHIMG ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

set(Hedgehog_INCLUDE_DIR ./lib/AtomicHedeghog/)

# added tclap
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/)

# Need to defined variable Hedgehog_INCLUDE_DIR to the hedgehog path
# hedgehog
find_package(Hedgehog REQUIRED)
find_package(X11 QUIET)
find_package(CUDA QUIET)
find_package(NVTX QUIET)

if (CUDA_FOUND)
	message(STATUS "CUDA found")
	set(CUDA_PROPAGATE_HOST_FLAGS OFF)
	set(CUDA_NVCC_FLAGS_RELEASE -O3; -DNDEBUG)
endif (CUDA_FOUND)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${Hedgehog_CXX_FLAGS})

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Werror -pedantic -g")
endif (NOT MSVC)

# files of the project
set(hh_img_files
    src/hhimg/concrete/chain_algorithm.h
    src/hhimg/concrete/data/pixel_value.h
    src/hhimg/concrete/data/mask.h
    src/hhimg/algorithm/gray_scale.h
    src/hhimg/algorithm/non_maximum_suppression.h
    src/hhimg/algorithm/crop.h
    src/hhimg/algorithm/minus.h
    src/hhimg/algorithm/convolute.h
    src/hhimg/tools/concepts.h
    src/hhimg/tools/shared_ptr_operators.h
    src/hhimg/tools/pixel_operators.h
    src/hhimg/tools/algorithm_operators.h
    src/hhimg/tools/utils.h
    src/hhimg/hhimg.h
    src/hhimg/abstract/abstract_algorithm.h
    src/hhimg/abstract/abstract_image_factory.h
    src/hhimg/abstract/data/abstract_pixel.h
    src/hhimg/abstract/data/abstract_image.h
    src/impl/cimg/cimg_pixel.h
    src/impl/cimg/cimg_image_factory.h
    src/impl/cimg/cimg_image.h
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

# main executable
add_executable(hh-img src/main.cc ${hh_img_files})
target_link_libraries(hh-img ${X11_LIBRARIES})
