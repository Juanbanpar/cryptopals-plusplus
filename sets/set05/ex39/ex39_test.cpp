#include <gtest/gtest.h>
#include "ex39.h"
#include <stdexcept>

TEST(Ex39, RSAKeyMathAndRoundTrip)
{
    const auto key_pair = set05::ex39::key_pair_from_primes(
        bignum::BigUint(47),
        bignum::BigUint(59)
    );
    EXPECT_EQ(key_pair.public_key.e, bignum::BigUint(3));
    EXPECT_EQ(key_pair.public_key.n, bignum::BigUint(2773));
    EXPECT_EQ(key_pair.private_key.d, bignum::BigUint(1779));

    const bignum::BigUint message(42);
    const auto ciphertext = set05::ex39::encrypt(message, key_pair.public_key);
    EXPECT_EQ(set05::ex39::decrypt(ciphertext, key_pair.private_key), message);
}

TEST(Ex39, RSAEncryptsAndDecryptsBytes)
{
    const auto key_pair = set05::ex39::generate_key_pair(64);
    const std::string text = "RSA message";
    const std::vector<unsigned char> message(text.begin(), text.end());
    const auto ciphertext = set05::ex39::encrypt_message(message, key_pair.public_key);
    EXPECT_EQ(
        set05::ex39::decrypt_message(ciphertext, key_pair.private_key, message.size()),
        message
    );
}

TEST(Ex39, RejectsCompositeKeyInputs)
{
    EXPECT_THROW(
        set05::ex39::key_pair_from_primes(bignum::BigUint(15), bignum::BigUint(59)),
        std::invalid_argument
    );
}
