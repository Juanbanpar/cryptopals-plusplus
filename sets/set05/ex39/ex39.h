#pragma once

#include "bignum.h"
#include <cstddef>

namespace set05::ex39
{

    using BigUint = bignum::BigUint;

    struct PublicKey
    {
        BigUint e;
        BigUint n;
    };

    struct PrivateKey
    {
        BigUint d;
        BigUint n;
    };

    struct KeyPair
    {
        PublicKey public_key;
        PrivateKey private_key;
        BigUint p;
        BigUint q;
    };

    KeyPair key_pair_from_primes(const BigUint &p, const BigUint &q);
    KeyPair generate_key_pair(size_t prime_bits = 256);

    BigUint encrypt(const BigUint &message, const PublicKey &key);
    BigUint decrypt(const BigUint &ciphertext, const PrivateKey &key);

    BigUint bytes_to_integer(const std::vector<unsigned char> &message);
    std::vector<unsigned char> integer_to_bytes(
        const BigUint &message,
        size_t width = 0
    );

    BigUint encrypt_message(
        const std::vector<unsigned char> &message,
        const PublicKey &key
    );

    std::vector<unsigned char> decrypt_message(
        const BigUint &ciphertext,
        const PrivateKey &key,
        size_t width = 0
    );

} // namespace set05::ex39
