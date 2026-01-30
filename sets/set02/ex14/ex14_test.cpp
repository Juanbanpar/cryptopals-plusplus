#include <gtest/gtest.h>
#include "ex14.h"

TEST(BreakECBHarder, Example)
{
    auto oracle = set02::ex14::create_oracle();
    std::string secret = set02::ex14::break_ecb_harder(oracle);
    ASSERT_NE(secret.find(
        "Rollin' in my 5.0\nWith my rag-top down so my hair can blow\nThe girlies on standby waving just to say hi\nDid you stop? No, I just drove by"),
        std::string::npos
    );
}
