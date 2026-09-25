#include <gtest/gtest.h>

#include "ex44.h"

#include <fstream>

#ifndef EX44_RESOURCE
#define EX44_RESOURCE "resources/set06/ex44/44.txt"
#endif

TEST(Ex44, RecoversPrivateKeyFromChallengeSignatures)
{
    std::ifstream input(EX44_RESOURCE);
    ASSERT_TRUE(input.good());
    const auto messages = set06::ex44::load_signatures(input);
    ASSERT_EQ(messages.size(), 11U);
    EXPECT_EQ(messages.front().message.back(), ' ');

    const auto recovery = set06::ex44::recover_reused_nonce(
        messages, set06::ex44::challenge_public_key()
    );
    ASSERT_TRUE(recovery.found);
    EXPECT_EQ(
        set06::dsa::private_key_fingerprint(recovery.private_key),
        "ca8f6f7c66fa362d40760d135b763eb8527d3d52"
    );
}
