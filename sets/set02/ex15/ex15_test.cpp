#include <gtest/gtest.h>
#include "ex15.h"

TEST(Ex15, ValidPadding)
{
    std::string s = "ICE ICE BABY\x04\x04\x04\x04";
    EXPECT_NO_THROW(set02::ex15::validate_and_strip_padding_str(s));
    EXPECT_EQ(set02::ex15::validate_and_strip_padding_str(s), "ICE ICE BABY");
}

TEST(Ex15, InvalidPaddingValues)
{
    std::string s = "ICE ICE BABY\x05\x05\x05\x05";
    EXPECT_THROW(set02::ex15::validate_and_strip_padding_str(s), std::invalid_argument);
}

TEST(Ex15, InvalidPaddingDifferentBytes)
{
    std::string s = "ICE ICE BABY\x01\x02\x03\x04";
    EXPECT_THROW(set02::ex15::validate_and_strip_padding_str(s), std::invalid_argument);
}
