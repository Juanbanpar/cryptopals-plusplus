#include <gtest/gtest.h>

#include "ex46.h"

TEST(Ex46, RecoversSmallPlaintextFromParityOnly)
{
    const auto keys = set05::ex39::key_pair_from_primes(
        bignum::BigUint(47), bignum::BigUint(59)
    );
    const bignum::BigUint message(65);
    const auto ciphertext = set05::ex39::encrypt(message, keys.public_key);
    const auto oracle = [&keys](const bignum::BigUint &candidate) {
        return set05::ex39::decrypt(candidate, keys.private_key).is_odd();
    };

    EXPECT_EQ(set06::ex46::recover_plaintext(ciphertext, keys.public_key, oracle), message);
}
