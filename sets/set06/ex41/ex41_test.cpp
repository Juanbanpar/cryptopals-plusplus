#include <gtest/gtest.h>

#include "ex41.h"

#include <stdexcept>

TEST(Ex41, RecoversPlaintextWithDifferentCiphertext)
{
    const auto keys = set05::ex39::key_pair_from_primes(
        bignum::BigUint(47), bignum::BigUint(59)
    );
    const bignum::BigUint message(42);
    const auto ciphertext = set05::ex39::encrypt(message, keys.public_key);
    set06::ex41::DecryptionOracle oracle(keys.private_key);

    EXPECT_EQ(
        set06::ex41::recover_plaintext(ciphertext, keys.public_key, oracle, bignum::BigUint(2)),
        message
    );
    const auto blinded = (ciphertext * bignum::mod_pow(
                                    bignum::BigUint(2),
                                    keys.public_key.e,
                                    keys.public_key.n
                                )) % keys.public_key.n;
    EXPECT_THROW(oracle.decrypt_once(blinded), std::invalid_argument);
}
