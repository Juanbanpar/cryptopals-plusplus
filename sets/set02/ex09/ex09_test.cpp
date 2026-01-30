#include "ex09.h"
#include <gtest/gtest.h>

TEST(PKCS7Pad, Example)
{
    std::string input = "YELLOW SUBMARINE";
    std::string expected = "YELLOW SUBMARINE\x04\x04\x04\x04";
    auto out = set02::ex09::pkcs7_pad_string(input, 20);
    EXPECT_EQ(out, expected);
}
