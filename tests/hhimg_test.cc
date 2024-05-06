#include "test_impl/rgb_value.h"
#include "test_impl/test_image.h"
#include "test_impl/test_pixel.h"
#include <gtest/gtest.h>
#include <hhimg/hhimg.h>
#include <memory>

TEST(Set, Pixels) {
    unsigned char r1 = 0, g1 = 0, b1 = 0;
    TestPixel<unsigned char> p1(r1, g1, b1);

    // change the references
    p1.set(10, 20, 30);
    ASSERT_EQ(r1, 10);
    ASSERT_EQ(g1, 20);
    ASSERT_EQ(b1, 30);
}

TEST(AffectationOperators, Pixels) {
    unsigned char r1 = 10, g1 = 20, b1 = 30;
    unsigned char r2 = 1, g2 = 2, b2 = 3;
    auto p1 = std::make_shared<TestPixel<unsigned char>>(r1, g1, b1);
    auto p2 = std::make_shared<TestPixel<unsigned char>>(r2, g2, b2);

    p1 -= p2;
    ASSERT_EQ(r1, 9);
    ASSERT_EQ(g1, 18);
    ASSERT_EQ(b1, 27);
    ASSERT_EQ(r2, 1);
    ASSERT_EQ(g2, 2);
    ASSERT_EQ(b2, 3);

    p1 += p2;
    ASSERT_EQ(r1, 10);
    ASSERT_EQ(g1, 20);
    ASSERT_EQ(b1, 30);

    p1 /= p2;
    ASSERT_EQ(r1, 10);
    ASSERT_EQ(g1, 10);
    ASSERT_EQ(b1, 10);

    p1 *= p2;
    ASSERT_EQ(r1, 10);
    ASSERT_EQ(g1, 20);
    ASSERT_EQ(b1, 30);
}

TEST(ArithmeticOperators, Pixels) {
    unsigned char r1 = 10, g1 = 20, b1 = 30;
    unsigned char r2 = 1, g2 = 2, b2 = 3;
    unsigned char r3 = 0, g3 = 0, b3 = 0;
    auto p1 = std::make_shared<TestPixel<unsigned char>>(r1, g1, b1);
    auto p2 = std::make_shared<TestPixel<unsigned char>>(r2, g2, b2);
    auto p3 = std::make_shared<TestPixel<unsigned char>>(r3, g3, b3);

    p3->set(p1 + p2);
    ASSERT_EQ(r3, 11);
    ASSERT_EQ(g3, 22);
    ASSERT_EQ(b3, 33);

    p3->set(p1 - p2);
    ASSERT_EQ(r3, 9);
    ASSERT_EQ(g3, 18);
    ASSERT_EQ(b3, 27);

    p3->set(p1 * p2);
    ASSERT_EQ(r3, 10);
    ASSERT_EQ(g3, 40);
    ASSERT_EQ(b3, 90);

    p3->set(p1 / p2);
    ASSERT_EQ(r3, 10);
    ASSERT_EQ(g3, 10);
    ASSERT_EQ(b3, 10);

    p3->set(p1 + p2 - (p1 / p2));
    ASSERT_EQ(r3, 1);
    ASSERT_EQ(g3, 12);
    ASSERT_EQ(b3, 23);

    p3->set(0);
    p3 += p1 + p2 - (p1 / p2);
    ASSERT_EQ(r3, 1);
    ASSERT_EQ(g3, 12);
    ASSERT_EQ(b3, 23);
}

TEST(AtRead, Images) {
    constexpr size_t width = 10;
    constexpr size_t height = 10;
    RGBValue<unsigned char> *mem = randomRGBValues<unsigned char>(width, height);
    RGBValue<unsigned char> *constMem = randomRGBValues<unsigned char>(width, height);
    TestImage<unsigned char> image(mem, width, height);
    const TestImage<unsigned char> constImage(constMem, width, height);

    // access to pixels
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            ASSERT_EQ(image.at(x, y), mem[y * width + x]);
            ASSERT_EQ(constImage.at(x, y), constMem[y * width + x]);
        }
    }
}

TEST(AtWrite, Images) {
    constexpr size_t width = 10;
    constexpr size_t height = 10;
    RGBValue<unsigned char> *mem = randomRGBValues<unsigned char>(width, height);
    TestImage<unsigned char> image(mem, width, height);

    // modifications
    image.at(0, 0)->set(10, 20, 30);
    image.at(9, 9)->set(10, 20, 30);
    image.at(3, 5)->set(0, 0, 0);
    image.at(3, 5)->set(image.at(0, 0) + image.at(9, 9));
    image.at(5, 3)->set(1, 2, 3);
    image.at(9, 9) += image.at(5, 3);

    ASSERT_EQ(mem[0], RGBValue<unsigned char>(10, 20, 30));
    ASSERT_EQ(mem[5 * width + 3], RGBValue<unsigned char>(20, 40, 60));
    ASSERT_EQ(mem[3 * width + 5], RGBValue<unsigned char>(1, 2, 3));
    ASSERT_EQ(mem[9 * width + 9], RGBValue<unsigned char>(11, 22, 33));
}

TEST(AffectationOperators, Images) {
    // TODO
}

TEST(ArithmeticOperators, Images) {
    // TODO
}

TEST(OneOperation, Algorithms) {
    constexpr size_t width = 10;
    constexpr size_t height = 10;
    RGBValue<unsigned char> *mem = new RGBValue<unsigned char>[width * height];
    auto image = std::make_shared<TestImage<unsigned char>>(mem, width, height);

    for (size_t i = 0; i < width * height; ++i) {
        ASSERT_FALSE(isGrayScaled(mem, width, height));
    }

    image |= hhimg::GrayScale<unsigned char>();

    // should we test the calculus ?
    for (size_t i = 0; i < width * height; ++i) {
        ASSERT_TRUE(isGrayScaled(mem, width, height));
    }
}

TEST(MultiplePipedOperation, Algorithms) {
    // TODO
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
