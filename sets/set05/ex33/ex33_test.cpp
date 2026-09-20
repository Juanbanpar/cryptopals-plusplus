#include <gtest/gtest.h>
#include "ex33.h"

TEST(Ex33, ToyDiffieHellman)
{
    const auto parameters = set05::ex33::toy_parameters();
    const auto alice = set05::ex33::generate_key_pair(parameters, 15);
    const auto bob = set05::ex33::generate_key_pair(parameters, 13);

    EXPECT_EQ(
        alice.public_key,
        bignum::mod_pow(parameters.g, bignum::BigUint(15), parameters.p)
    );
    const auto alice_secret = set05::ex33::derive_shared_secret(
        parameters, alice.private_key, bob.public_key
    );
    const auto bob_secret = set05::ex33::derive_shared_secret(
        parameters, bob.private_key, alice.public_key
    );
    EXPECT_EQ(alice_secret, bob_secret);
    EXPECT_EQ(alice_secret, bignum::BigUint(29));
}

TEST(Ex33, NistSizedParameters)
{
    const auto parameters = set05::ex33::default_parameters();
    EXPECT_EQ(parameters.g, bignum::BigUint(2));
    EXPECT_EQ(parameters.p.bit_length(), 1536U);

    const auto alice = set05::ex33::generate_key_pair(parameters, 5);
    const auto bob = set05::ex33::generate_key_pair(parameters, 7);
    EXPECT_EQ(
        set05::ex33::derive_shared_secret(parameters, alice.private_key, bob.public_key),
        set05::ex33::derive_shared_secret(parameters, bob.private_key, alice.public_key)
    );
}
