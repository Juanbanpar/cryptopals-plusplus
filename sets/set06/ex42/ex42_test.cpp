#include <gtest/gtest.h>

#include "ex42.h"

TEST(Ex42, ForgesSha1SignatureWithE3)
{
    const auto modulus = (bignum::BigUint(1) << 1024) - bignum::BigUint(1);
    const set05::ex39::PublicKey key{bignum::BigUint(3), modulus};
    const std::string text = "hi mom";
    const std::vector<unsigned char> message(text.begin(), text.end());
    const auto signature = set06::ex42::forge_signature(message, key, 128);

    EXPECT_TRUE(set06::ex42::verify_signature_weakly(message, signature, key, 128));
    EXPECT_FALSE(set06::ex42::verify_signature_weakly(
        std::vector<unsigned char>{'n', 'o'}, signature, key, 128
    ));
}
