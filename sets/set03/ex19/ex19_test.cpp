#include <gtest/gtest.h>
#include "ex19.h"
#include <algorithm>
#include <cctype>

namespace
{
    bool contains_case_insensitive(const std::string &text, const std::string &needle)
    {
        auto lower = [](std::string value)
        {
            std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c)
                           { return static_cast<char>(std::tolower(c)); });
            return value;
        };

        return lower(text).find(lower(needle)) != std::string::npos;
    }
}

TEST(BreakFixedNonceCTR, Example)
{
    auto ciphertexts = set03::ex19::encrypt_plaintexts();
    auto plaintexts = set03::ex19::break_fixed_nonce_ctr(ciphertexts);

    EXPECT_EQ(plaintexts.size(), 40);
    ASSERT_TRUE(contains_case_insensitive(plaintexts[0], "I have met them at close of day"));
    ASSERT_TRUE(contains_case_insensitive(plaintexts[1], "Coming with vivid faces"));
    ASSERT_TRUE(contains_case_insensitive(plaintexts[2], "From counter or desk among grey"));
    ASSERT_TRUE(contains_case_insensitive(plaintexts[3], "Eighteenth-century houses."));
    // ... additional checks can be added here for other plaintexts
    ASSERT_TRUE(contains_case_insensitive(plaintexts[39], "A terrible beauty is born."));
}
