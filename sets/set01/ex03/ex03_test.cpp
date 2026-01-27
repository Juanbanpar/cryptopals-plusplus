#include "ex03.h"
#include <gtest/gtest.h>

TEST(SingleByteXOR, Example) {
  const std::string hex = "1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
  const std::string expected = "Cooking MC's like a pound of bacon";
  auto [key, plain] = set01::ex03::crack_single_byte_xor(hex);
  EXPECT_EQ(plain, expected);
}
