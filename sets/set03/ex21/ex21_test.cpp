#include <gtest/gtest.h>
#include "ex21.h"

TEST(Ex21, KnownValues)
{
    // Test against known MT19937 output for seed=0
    set03::ex21::MT19937 rng(0);

    // First few values from MT19937 with seed 0 (reference implementation)
    EXPECT_EQ(rng(), 2357136044u);
    EXPECT_EQ(rng(), 2546248239u);
    EXPECT_EQ(rng(), 3071714933u);
    EXPECT_EQ(rng(), 3626093760u);
    EXPECT_EQ(rng(), 2588848963u);
}

TEST(Ex21, Reproducible)
{
    set03::ex21::MT19937 rng1(42);
    set03::ex21::MT19937 rng2(42);

    for (int i = 0; i < 100; ++i)
    {
        EXPECT_EQ(rng1(), rng2());
    }
}
