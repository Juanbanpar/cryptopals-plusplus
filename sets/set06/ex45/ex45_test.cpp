#include <gtest/gtest.h>

#include "ex45.h"

TEST(Ex45, ZeroGeneratorAcceptsUnrelatedMessage)
{
    const auto parameters = set06::dsa::challenge_parameters();
    const auto keys = set06::dsa::key_pair_from_private(parameters, bignum::BigUint(23));
    auto tampered = parameters;
    tampered.g = bignum::BigUint(0);
    const auto broken_keys = set06::dsa::key_pair_from_private(tampered, keys.private_key.x);
    const std::vector<unsigned char> signed_message{'h', 'e', 'l', 'l', 'o'};
    const std::vector<unsigned char> other_message{'g', 'o', 'o', 'd', 'b', 'y', 'e'};
    const auto signature = set06::dsa::sign(
        signed_message, broken_keys.private_key, bignum::BigUint(29)
    );

    EXPECT_EQ(signature.r, bignum::BigUint(0));
    EXPECT_TRUE(set06::dsa::verify(other_message, signature, broken_keys.public_key));
}

TEST(Ex45, PPlusOneAllowsUniversalMagicSignatures)
{
    const auto parameters = set06::dsa::challenge_parameters();
    const auto keys = set06::dsa::key_pair_from_private(parameters, bignum::BigUint(31));
    auto tampered = parameters;
    tampered.g = parameters.p + bignum::BigUint(1);
    const set06::dsa::PublicKey vulnerable{tampered, keys.public_key.y};
    const auto signature = set06::ex45::forge_universal_signature(
        vulnerable, bignum::BigUint(37)
    );

    EXPECT_TRUE(set06::dsa::verify(
        std::vector<unsigned char>{'H', 'e', 'l', 'l', 'o'}, signature, vulnerable
    ));
    EXPECT_TRUE(set06::dsa::verify(
        std::vector<unsigned char>{'G', 'o', 'o', 'd', 'b', 'y', 'e'}, signature, vulnerable
    ));
}
