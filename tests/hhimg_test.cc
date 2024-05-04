#include "test_pixel.h"
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
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
