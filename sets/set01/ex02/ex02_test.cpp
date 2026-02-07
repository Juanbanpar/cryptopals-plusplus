#include "ex02.h"
#include <gtest/gtest.h>

TEST(Ex02, FixedXOR)
{
    const std::string a = "1c0111001f010100061a024b53535009181c";
    const std::string b = "686974207468652062756c6c277320657965";
    const std::string expected = "746865206b696420646f6e277420706c6179";
    EXPECT_EQ(fixed_xor_hex(a, b), expected);
}
