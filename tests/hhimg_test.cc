#include "test_impl/rgb_value.h"
#include "test_impl/test_image.h"
#include "test_impl/test_image_factory.h"
#include <gtest/gtest.h>
#include <hhimg/hhimg.h>
#include <memory>

TEST(Images, ReadAccessor) {
    constexpr size_t width = 10;
    constexpr size_t height = 10;
    RGBValue<unsigned char> *mem =
        randomRGBValues<unsigned char>(width, height);
    RGBValue<unsigned char> *constMem =
        randomRGBValues<unsigned char>(width, height);
    TestImage<unsigned char> image(mem, width, height);
    const TestImage<unsigned char> constImage(constMem, width, height);
    std::shared_ptr<hhimg::AbstractImage<unsigned char>> p =
        std::make_shared<TestImage<unsigned char>>(mem, width, height);

    // access to pixels
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            ASSERT_EQ(image.red(x, y), mem[y * width + x].red);
            ASSERT_EQ(image.green(x, y), mem[y * width + x].green);
            ASSERT_EQ(image.blue(x, y), mem[y * width + x].blue);
            ASSERT_EQ(constImage.red(x, y), constMem[y * width + x].red);
            ASSERT_EQ(constImage.green(x, y), constMem[y * width + x].green);
            ASSERT_EQ(constImage.blue(x, y), constMem[y * width + x].blue);
        }
    }

    // free memory
    delete[] mem;
    delete[] constMem;
}

TEST(Images, WriteAccessors) {
    constexpr size_t width = 10;
    constexpr size_t height = 10;
    RGBValue<unsigned char> *mem =
        randomRGBValues<unsigned char>(width, height);
    TestImage<unsigned char> image(mem, width, height);

    // modifications
    image.set(0, 0, 10, 20, 30);
    image.set(9, 9, 20, 40, 60);
    image.set(3, 5, 1, 2, 3);

    ASSERT_EQ(mem[0], RGBValue<unsigned char>(10, 20, 30));
    ASSERT_EQ(mem[9 * width + 9], RGBValue<unsigned char>(20, 40, 60));
    ASSERT_EQ(mem[5 * width + 3], RGBValue<unsigned char>(1, 2, 3));

    // free memory
    delete[] mem;
}

TEST(Algorithms, OneOperation) {
    constexpr size_t width = 10;
    constexpr size_t height = 10;
    RGBValue<unsigned char> *mem =
        randomRGBValues<unsigned char>(width, height);
    auto image = std::make_shared<TestImage<unsigned char>>(mem, width, height);

    ASSERT_FALSE(isGrayScaled(mem, width, height));

    image |= hhimg::GrayScale<unsigned char>();

    // should we test the calculus ?
    ASSERT_TRUE(isGrayScaled(mem, width, height));

    // free memory
    delete[] mem;
}

TEST(Algorithms, MultiplePipedOperation) {
    RGBValue<unsigned char> wht = {255, 255, 255};
    RGBValue<unsigned char> blk = {0, 0, 0};
    constexpr size_t width = 3;
    constexpr size_t height = 3;
    // clang-format off
    RGBValue<unsigned char> mem[] = {
        wht, wht, wht,
        wht, blk, wht,
        wht, wht, wht,
    };
    // clang-format on
    auto image = std::make_shared<TestImage<unsigned char>>(mem, width, height);
    std::vector<double> v(9, 1.0 / 9);
    hhimg::Mask<double> meanFilter(v, 3, 3);
    auto imgFactory = std::make_shared<TestImageFactory<unsigned char>>();

    image |=
        hhimg::GrayScale<unsigned char>() |
        hhimg::Convolute<unsigned char, double>(imgFactory, meanFilter) |
        hhimg::NonMaximumSuppression<unsigned char>(20); // blk => value < 20

    for (size_t y = 0; y < image->height(); ++y) {
        for (size_t x = 0; x < image->width(); ++x) {
            // the black dot has been erased
            ASSERT_EQ(image->red(x, y), wht.red);
            ASSERT_EQ(image->green(x, y), wht.green);
            ASSERT_EQ(image->blue(x, y), wht.blue);
        }
    }
}

TEST(Algorithms, Minus) {
    RGBValue<unsigned char> wht = {255, 255, 255};
    RGBValue<unsigned char> blk = {0, 0, 0};
    constexpr size_t width = 3;
    constexpr size_t height = 3;
    // clang-format off
    RGBValue<unsigned char> mem1[] = {
        wht, blk, wht,
        blk, wht, blk,
        wht, blk, wht,
    };
    RGBValue<unsigned char> mem2[] = {
        blk, blk, blk,
        blk, blk, blk,
        blk, blk, blk,
    };
    RGBValue<unsigned char> mem3[] = {
        wht, wht, wht,
        wht, wht, wht,
        wht, wht, wht,
    };
    // clang-format on
    auto image1 =
        std::make_shared<TestImage<unsigned char>>(mem1, width, height);
    auto image2 =
        std::make_shared<TestImage<unsigned char>>(mem2, width, height);
    auto image3 =
        std::make_shared<TestImage<unsigned char>>(mem3, width, height);

    for (size_t i = 0; i < width * height; ++i) {
        if (i % 2 == 0) {
            ASSERT_EQ(image1->red(i), wht.red);
            ASSERT_EQ(image1->green(i), wht.green);
            ASSERT_EQ(image1->blue(i), wht.blue);
        } else {
            ASSERT_EQ(image1->red(i), blk.red);
            ASSERT_EQ(image1->green(i), blk.green);
            ASSERT_EQ(image1->blue(i), blk.blue);
        }
    }

    image1 |= hhimg::Minus<unsigned char>(image2);

    // nothing has changed
    for (size_t i = 0; i < width * height; ++i) {
        if (i % 2 == 0) {
            ASSERT_EQ(image1->red(i), wht.red);
            ASSERT_EQ(image1->green(i), wht.green);
            ASSERT_EQ(image1->blue(i), wht.blue);
        } else {
            ASSERT_EQ(image1->red(i), blk.red);
            ASSERT_EQ(image1->green(i), blk.green);
            ASSERT_EQ(image1->blue(i), blk.blue);
        }
    }

    image1 |= hhimg::Minus<unsigned char>(image3);

    // everything is black
    for (size_t i = 0; i < width * height; ++i) {
        ASSERT_EQ(image1->red(i), blk.red);
        ASSERT_EQ(image1->green(i), blk.green);
        ASSERT_EQ(image1->blue(i), blk.blue);
    }
}

TEST(Algorithms, Crop) {
    RGBValue<unsigned char> wht = {255, 255, 255};
    RGBValue<unsigned char> blk = {0, 0, 0};
    constexpr size_t width = 3;
    constexpr size_t height = 3;
    // clang-format off
    RGBValue<unsigned char> mem[] = {
        blk, blk, blk,
        blk, wht, blk,
        blk, blk, blk,
    };
    // clang-format on
    auto image = std::make_shared<TestImage<unsigned char>>(mem, width, height);
    auto imgFactory = std::make_shared<TestImageFactory<unsigned char>>();

    image |= hhimg::Crop<unsigned char>(imgFactory, 1, 1, 1, 1);

    ASSERT_EQ(image->width(), 1);
    ASSERT_EQ(image->height(), 1);
    ASSERT_EQ(image->red(0, 0), wht.red);
    ASSERT_EQ(image->green(0, 0), wht.green);
    ASSERT_EQ(image->blue(0, 0), wht.blue);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
