#include <gtest/gtest.h>
#include "common.h"
#include "ex31.h"

TEST(Ex31, HMACSHA1KnownVector)
{
    const std::vector<unsigned char> key(20, 0x0b);
    const std::vector<unsigned char> message{'H', 'i', ' ', 'T', 'h', 'e', 'r', 'e'};
    const auto mac = set04::ex31::hmac_sha1(key, message);

    EXPECT_EQ(bytes_to_hex(mac), "b617318655057264e28bc0b6fb378c8ef146be00");
}

TEST(Ex31, InsecureComparisonAndTimingAttack)
{
    const std::string file = "foo";
    const std::vector<unsigned char> key{'t', 'i', 'm', 'i', 'n', 'g'};
    set04::ex31::TimingOracle oracle(key, set04::ex31::Timing(50000));
    const auto expected = set04::ex31::hmac_sha1(key, std::vector<unsigned char>{'f', 'o', 'o'});
    auto candidate = expected;
    candidate[0] ^= 1;

    EXPECT_TRUE(oracle.verify(file, expected));
    EXPECT_FALSE(oracle.verify(file, candidate));
    EXPECT_GT(oracle.measure(file, expected), oracle.measure(file, candidate));

    const auto recovered = set04::ex31::recover_hmac_sha1(
        file,
        [&oracle](const auto &name, const auto &signature)
        { return oracle.measure(name, signature); }
    );
    EXPECT_EQ(recovered, expected);
}
