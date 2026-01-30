#include <gtest/gtest.h>
#include "ex11.h"
#include "../../lib/common/common.h"

TEST(OracleDetection, 20Rounds)
{
    for (int i = 0; i < 20; ++i)
    {
        std::string actual_mode;
        auto oracle = [&](const std::vector<unsigned char> &input)
        {
            return set02::ex11::encryption_oracle(input, &actual_mode);
        };
        std::string detected = detect_ecb_or_cbc(oracle);
        EXPECT_EQ(actual_mode, detected);
    }
}
