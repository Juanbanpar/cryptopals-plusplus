#include <gtest/gtest.h>
#include "ex31.h"
#include "ex32.h"

TEST(Ex32, RecoverHMACWithLessArtificialLeak)
{
    const std::string file = "foo";
    const std::vector<unsigned char> key{'l', 'e', 's', 's', ' ' ,'l', 'e', 'a', 'k'};
    set04::ex31::TimingOracle oracle(key, set04::ex31::Timing(5000), set04::ex31::Timing(500));
    const auto expected = set04::ex31::hmac_sha1(key, std::vector<unsigned char>{'f', 'o', 'o'});
    const auto recovered = set04::ex32::recover_hmac_sha1(file, oracle, 5);

    EXPECT_EQ(recovered, expected);
    EXPECT_TRUE(oracle.verify(file, recovered));
}
