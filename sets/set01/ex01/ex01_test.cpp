#include "ex01.h"
#include <gtest/gtest.h>

TEST(AddTest, Basic) {
  EXPECT_EQ(add(2, 3), 5);
  EXPECT_EQ(add(-1, 1), 0);
}
