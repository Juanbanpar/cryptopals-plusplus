#include <gtest/gtest.h>
#include "ex20.h"

TEST(BreakFixedNonceCTRStatistically, Example)
{
    auto ciphertexts = set03::ex20::encrypt_plaintexts();
    auto plaintexts = set03::ex20::break_fixed_nonce_ctr(ciphertexts);

    EXPECT_EQ(plaintexts.size(), 60);
    ASSERT_NE(plaintexts[0].find("N'm rated \"R\"...this is a warning, ya better void / P"), std::string::npos);
    ASSERT_NE(plaintexts[1].find("Duz I came back to attack others in spite- / Strike l"), std::string::npos);
    ASSERT_NE(plaintexts[2].find("Eut don't be afraid in the dark, in a park / Not a sc"), std::string::npos);
    ASSERT_NE(plaintexts[3].find("^a tremble like a alcoholic, muscles tighten up / Wha"), std::string::npos);
    // ... additional checks can be added here for other plaintexts
    ASSERT_NE(plaintexts[59].find("Fnd we outta here / Yo, what happened to peace? / Pea"), std::string::npos);
}
