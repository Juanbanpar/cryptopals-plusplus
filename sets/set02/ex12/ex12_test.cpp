#include <gtest/gtest.h>
#include "ex12.h"

TEST(BreakECB, Example)
{
    auto oracle = set02::ex12::create_oracle();
    std::string secret = set02::ex12::break_ecb_simple(oracle);
    ASSERT_NE(secret.find(
        "Rollin' in my 5.0\nWith my rag-top down so my hair can blow\nThe girlies on standby waving just to say hi\nDid you stop? No, I just drove by"),
        std::string::npos
    );
}
