#include "test_impl/rgb_value.h"
#include "test_impl/test_image.h"
#include "test_impl/test_image_factory.h"
#include <gtest/gtest.h>
#include <hhimg/hhimg.h>
#include <memory>

using PixelType = unsigned char;

TEST(Pixels, Operators) {
    hhimg::Pixel<PixelType> p1 = {1, 2, 3};
    hhimg::Pixel<PixelType> p2 = {10, 20, 30};
    hhimg::Pixel<PixelType> p3 = {10, 10, 10};

    // pixel Op pixel
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p1 + p2), hhimg::pixel<PixelType>(11, 22, 33));
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p2 - p1), hhimg::pixel<PixelType>(9, 18, 27));
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p2 - p2), hhimg::pixel<PixelType>(0));
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p2 * p1), hhimg::pixel<PixelType>(10, 40, 90));
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p1 * p1), hhimg::pixel<PixelType>(1, 4, 9));
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p2 / p2), hhimg::pixel<PixelType>(1));
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p2 / p1), hhimg::pixel<PixelType>(10));

    // with values
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p1 + 4), hhimg::pixel<PixelType>(5, 6, 7));
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p2 - 4), hhimg::pixel<PixelType>(6, 16, 26));
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p3 * 4), hhimg::pixel<PixelType>(40));
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p3 / 10), hhimg::pixel<PixelType>(1));

    // multimple operations
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p2 / p1 + p3), hhimg::pixel<PixelType>(20));
    ASSERT_EQ(hhimg::pixelCast<PixelType>(p2 / p1 * p3 - p2), hhimg::pixel<PixelType>(90, 80, 70));
    ASSERT_EQ(hhimg::pixelCast<PixelType>(1 + p2 / p1 * 10), hhimg::pixel<PixelType>(101));
}

TEST(Pixels, AffactionOperators) {
    hhimg::Pixel<PixelType> p1 = {1, 2, 3};
    hhimg::Pixel<PixelType> p2 = {10, 20, 30};
    hhimg::Pixel<PixelType> p3 = {10, 10, 10};

    // pixel Op pixel
    p1 += p2;
    p2 -= p3 - (p1 - p2);
    p3 *= 4;
    p3 /= 10;

    std::cout << p2 << std::endl;

    ASSERT_EQ(p1, hhimg::pixel<PixelType>(11, 22, 33));
    ASSERT_EQ(p2, hhimg::pixel<PixelType>(1, 12, 23));
    ASSERT_EQ(p3, hhimg::pixel<PixelType>(4));
}

TEST(Images, ReadAccessor) {
    constexpr size_t width = 10;
    constexpr size_t height = 10;
    RGBValue<PixelType> *mem = randomRGBValues<PixelType>(width, height);
    RGBValue<PixelType> *constMem = randomRGBValues<PixelType>(width, height);
    TestImage<PixelType> image(mem, width, height);
    const TestImage<PixelType> constImage(constMem, width, height);
    std::shared_ptr<hhimg::AbstractImage<PixelType>> p =
        std::make_shared<TestImage<PixelType>>(mem, width, height);

    // access to pixels
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            ASSERT_EQ(image.at(x, y), mem[y * width + x]);
            ASSERT_EQ(constImage.at(x, y), constMem[y * width + x]);
        }
    }

    // free memory
    delete[] mem;
    delete[] constMem;
}

TEST(Images, WriteAccessors) {
    constexpr size_t width = 10;
    constexpr size_t height = 10;
    RGBValue<PixelType> *mem = randomRGBValues<PixelType>(width, height);
    TestImage<PixelType> image(mem, width, height);

    // modifications
    image.set(0, 0, 10, 20, 30);
    image.set(9, 9, 20, 40, 60);
    image.set(1, 0, 4);
    image.set(3, 5, 1, 2, 3);
    image.set(0, 1, {12, 13, 14});

    ASSERT_EQ(mem[0], RGBValue<PixelType>(10, 20, 30));
    ASSERT_EQ(mem[9 * width + 9], RGBValue<PixelType>(20, 40, 60));
    ASSERT_EQ(mem[1], RGBValue<PixelType>(4, 4, 4));
    ASSERT_EQ(mem[5 * width + 3], RGBValue<PixelType>(1, 2, 3));
    ASSERT_EQ(mem[1 * width], RGBValue<PixelType>(12, 13, 14));

    // free memory
    delete[] mem;
}

TEST(Algorithms, OneOperation) {
    constexpr size_t width = 10;
    constexpr size_t height = 10;
    RGBValue<PixelType> *mem = randomRGBValues<PixelType>(width, height);
    auto image = std::make_shared<TestImage<PixelType>>(mem, width, height);

    ASSERT_FALSE(isGrayScaled(mem, width, height));

    image |= hhimg::GrayScale<PixelType>();

    // should we test the calculus ?
    ASSERT_TRUE(isGrayScaled(mem, width, height));

    // free memory
    delete[] mem;
}

TEST(Algorithms, MultiplePipedOperation) {
    RGBValue<PixelType> wht = {255, 255, 255};
    RGBValue<PixelType> blk = {0, 0, 0};
    constexpr size_t width = 3;
    constexpr size_t height = 3;
    // clang-format off
    RGBValue<PixelType> mem[] = {
        wht, wht, wht,
        wht, blk, wht,
        wht, wht, wht,
    };
    // clang-format on
    auto image = std::make_shared<TestImage<PixelType>>(mem, width, height);
    std::vector<double> v(9, 1.0 / 9);
    hhimg::Mask<double> meanFilter(v, 3, 3);
    auto imgFactory = std::make_shared<TestImageFactory<PixelType>>();

    image |= hhimg::GrayScale<PixelType>() |
             hhimg::Convolute<PixelType, double>(imgFactory, meanFilter) |
             hhimg::NonMaximumSuppression<PixelType>(20); // blk => value < 20

    ASSERT_EQ(image->width(), 1);
    ASSERT_EQ(image->height(), 1);
    ASSERT_EQ(image->at(0, 0), wht);
}

TEST(Algorithms, Minus) {
    RGBValue<PixelType> wht = {255, 255, 255};
    RGBValue<PixelType> blk = {0, 0, 0};
    constexpr size_t width = 4;
    constexpr size_t height = 3;
    // clang-format off
    RGBValue<PixelType> mem1[] = {
        wht, blk, wht, blk,
        wht, blk, wht, blk,
        wht, blk, wht, blk,
    };
    RGBValue<PixelType> mem2[] = {
        blk, blk, blk, blk,
        blk, blk, blk, blk,
        blk, blk, blk, blk,
    };
    RGBValue<PixelType> mem3[] = {
        wht, wht, wht, wht,
        wht, wht, wht, wht,
        wht, wht, wht, wht,
    };
    // clang-format on
    auto image1 = std::make_shared<TestImage<PixelType>>(mem1, width, height);
    auto image2 = std::make_shared<TestImage<PixelType>>(mem2, width, height);
    auto image3 = std::make_shared<TestImage<PixelType>>(mem3, width, height);

    for (size_t i = 0; i < width * height; ++i) {
        if (i % 2 == 0) {
            ASSERT_EQ(image1->at(i), wht);
        } else {
            ASSERT_EQ(image1->at(i), blk);
        }
    }

    image1 |= hhimg::Minus<PixelType>(image2);

    // nothing has changed
    for (size_t i = 0; i < width * height; ++i) {
        if (i % 2 == 0) {
            ASSERT_EQ(image1->at(i), wht);
        } else {
            ASSERT_EQ(image1->at(i), blk);
        }
    }

    image1 |= hhimg::Minus<PixelType>(image3);

    // everything is black
    for (size_t i = 0; i < width * height; ++i) {
            ASSERT_EQ(image1->at(i), blk);
    }
}

TEST(Algorithms, Crop) {
    RGBValue<PixelType> wht = {255, 255, 255};
    RGBValue<PixelType> blk = {0, 0, 0};
    constexpr size_t width = 4;
    constexpr size_t height = 3;
    // clang-format off
    RGBValue<PixelType> mem[] = {
        blk, blk, blk, blk,
        blk, wht, wht, blk,
        blk, blk, blk, blk,
    };
    // clang-format on
    auto image = std::make_shared<TestImage<PixelType>>(mem, width, height);
    auto imgFactory = std::make_shared<TestImageFactory<PixelType>>();

    image |= hhimg::Crop<PixelType>(imgFactory, 1, 1, 2, 1);

    ASSERT_EQ(image->width(), 2);
    ASSERT_EQ(image->height(), 1);
    ASSERT_EQ(image->at(0, 0), wht);
    ASSERT_EQ(image->at(1, 0), wht);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
