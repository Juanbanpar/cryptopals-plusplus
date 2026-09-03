#include <gtest/gtest.h>
#include "ex23.h"

TEST(Ex23, Untemper)
{
    // Test that untemper inverts the tempering
    set03::ex21::MT19937 rng(42);

    for (int i = 0; i < 10; ++i)
    {
        uint32_t output = rng();
        uint32_t state_elem = set03::ex23::untemper(output);

        // Apply tempering to state element
        uint32_t y = state_elem;
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680;
        y ^= (y << 15) & 0xefc60000;
        y ^= (y >> 18);

        EXPECT_EQ(y, output);
    }
}

TEST(Ex23, CloneMT19937)
{
    set03::ex21::MT19937 original(99999);

    // Skip some values
    for (int i = 0; i < 500; ++i)
        original();

    auto clone = set03::ex23::clone_mt19937(original);

    // Verify next 1000 outputs match
    for (int i = 0; i < 1000; ++i)
    {
        EXPECT_EQ(original(), clone());
    }
}
