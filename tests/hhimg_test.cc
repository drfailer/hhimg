#include <gtest/gtest.h>
#include <hhimg/hhimg.h>

TEST(Basic, BasicTests) {
    ASSERT_EQ(1, 1);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
