#include "common.h"
#include <gtest/gtest.h>

TEST(HammingDistance, Example)
{
    std::string a = "this is a test";
    std::string b = "wokka wokka!!!";
    std::vector<unsigned char> va(a.begin(), a.end());
    std::vector<unsigned char> vb(b.begin(), b.end());
    int d = hamming_distance(va, vb);
    EXPECT_EQ(d, 37);
}
