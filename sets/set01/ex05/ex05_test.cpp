#include "ex05.h"
#include <gtest/gtest.h>

TEST(Ex05, RepeatingKeyXOR)
{
    const std::string text = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
    const std::string key = "ICE";
    const std::string expected = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
    EXPECT_EQ(set01::ex05::repeating_key_xor_hex_from_text(text, key), expected);
}
