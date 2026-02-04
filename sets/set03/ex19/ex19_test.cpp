#include <gtest/gtest.h>
#include "ex19.h"
#include <iostream>

TEST(BreakFixedNonceCTR, Example)
{
    auto ciphertexts = set03::ex19::encrypt_plaintexts();
    auto plaintexts = set03::ex19::break_fixed_nonce_ctr(ciphertexts);

    EXPECT_EQ(plaintexts.size(), 40);
    ASSERT_NE(plaintexts[0].find("I have met them at close of day"), std::string::npos);
    ASSERT_NE(plaintexts[1].find("Coming with vivid faces"), std::string::npos);
    ASSERT_NE(plaintexts[2].find("From counter or desk among grey"), std::string::npos);
    ASSERT_NE(plaintexts[3].find("Eighteenth-century houses."), std::string::npos);
    // ... additional checks can be added here for other plaintexts
    ASSERT_NE(plaintexts[39].find("A terrible beauty is born."), std::string::npos);
}
