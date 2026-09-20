#include <gtest/gtest.h>
#include "ex40.h"

TEST(Ex40, RecoversBroadcastPlaintextWithCRT)
{
    const std::array<set05::ex39::KeyPair, 3> recipients = {
        set05::ex39::key_pair_from_primes(bignum::BigUint(47), bignum::BigUint(59)),
        set05::ex39::key_pair_from_primes(bignum::BigUint(71), bignum::BigUint(83)),
        set05::ex39::key_pair_from_primes(bignum::BigUint(89), bignum::BigUint(101))};
    const bignum::BigUint message(42);
    const std::vector<unsigned char> encoded = set05::ex39::integer_to_bytes(message);
    const auto ciphertexts = set05::ex40::encrypt_for_recipients(encoded, recipients);

    EXPECT_EQ(set05::ex40::recover_message(ciphertexts), message);
    EXPECT_EQ(set05::ex40::recover_message_bytes(ciphertexts), encoded);
}

TEST(Ex40, BroadcastAttackRecoversText)
{
    const std::array<set05::ex39::KeyPair, 3> recipients = {
        set05::ex39::key_pair_from_primes(bignum::BigUint(101), bignum::BigUint(107)),
        set05::ex39::key_pair_from_primes(bignum::BigUint(113), bignum::BigUint(131)),
        set05::ex39::key_pair_from_primes(bignum::BigUint(137), bignum::BigUint(149))};
    const std::string text = "A";
    const std::vector<unsigned char> message(text.begin(), text.end());
    const auto ciphertexts = set05::ex40::encrypt_for_recipients(message, recipients);

    EXPECT_EQ(
        set05::ex40::recover_message_bytes(ciphertexts, message.size()),
        message
    );
}
