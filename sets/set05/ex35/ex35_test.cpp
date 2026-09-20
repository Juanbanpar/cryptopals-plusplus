#include <gtest/gtest.h>
#include "ex35.h"

TEST(Ex35, BreaksAllMaliciousGenerators)
{
    const auto parameters = set05::ex33::toy_parameters();
    const std::string text = "group negotiation";
    const std::vector<unsigned char> message(text.begin(), text.end());
    const std::vector<unsigned char> alice_iv(16, 0x31);
    const std::vector<unsigned char> bob_iv(16, 0x32);

    for (const auto &generator : {
             bignum::BigUint(1),
             parameters.p,
             parameters.p - bignum::BigUint(1)})
    {
        const auto result = set05::ex35::malicious_generator_attack(
            message, parameters, generator, alice_iv, bob_iv
        );
        EXPECT_EQ(result.decrypted_alice_message, message);
        EXPECT_EQ(result.decrypted_bob_message, message);
        if (generator == bignum::BigUint(1))
            EXPECT_EQ(result.predictable_secret, bignum::BigUint(1));
        else if (generator == parameters.p)
            EXPECT_EQ(result.predictable_secret, bignum::BigUint(0));
        else
            EXPECT_EQ(result.predictable_secret, parameters.p - bignum::BigUint(1));
    }
}
