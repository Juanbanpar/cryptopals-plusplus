#include <gtest/gtest.h>
#include "ex22.h"
#include "../ex21/ex21.h"
#include <chrono>

TEST(Ex22, CrackSeed)
{
    // Use a known seed and verify we can crack it
    uint32_t seed = 1234567890;
    set03::ex21::MT19937 rng(seed);
    uint32_t output = rng();

    // Simulate cracking from slightly after
    uint32_t cracked = set03::ex22::crack_seed(output, seed + 100, 200);
    EXPECT_EQ(cracked, seed);
}

TEST(Ex22, CrackRandomSeed)
{
    uint32_t actual_seed;
    uint32_t output = set03::ex22::generate_seeded_output(actual_seed);

    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    uint32_t time_now = (uint32_t)std::chrono::duration_cast<std::chrono::seconds>(epoch).count();

    uint32_t cracked = set03::ex22::crack_seed(output, time_now, 2500);
    EXPECT_EQ(cracked, actual_seed);
}
