#include <gtest/gtest.h>
#include "common.h"
#include "ex34.h"

TEST(Ex34, ParameterInjectionDecryptsBothMessages)
{
    const std::string text = "hello from Alice";
    const std::vector<unsigned char> message(text.begin(), text.end());
    const std::vector<unsigned char> alice_iv(16, 0x11);
    const std::vector<unsigned char> bob_iv(16, 0x22);
    const auto result = set05::ex34::parameter_injection_attack(
        message,
        set05::ex33::toy_parameters(),
        alice_iv,
        bob_iv
    );

    EXPECT_EQ(result.predictable_secret, bignum::BigUint(0));
    EXPECT_EQ(result.decrypted_alice_message, message);
    EXPECT_EQ(result.decrypted_bob_message, message);
    EXPECT_EQ(result.alice_to_bob.iv, alice_iv);
    EXPECT_EQ(result.bob_to_alice.iv, bob_iv);
}

TEST(Ex34, SharedSecretDerivesAes128Key)
{
    const auto key = set05::ex34::derive_aes_key(bignum::BigUint(0));
    EXPECT_EQ(key.size(), 16U);
    EXPECT_EQ(
        bytes_to_hex(key),
        "5ba93c9db0cff93f52b521d7420e43f6"
    );
}
