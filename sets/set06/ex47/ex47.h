#pragma once

#include "ex39.h"

#include <functional>

namespace set06::ex47
{

    using PaddingOracle = std::function<bool(const bignum::BigUint &)>;

    std::vector<unsigned char> pad_message(
        const std::vector<unsigned char> &message,
        size_t block_size
    );
    bignum::BigUint encrypt_padded(
        const std::vector<unsigned char> &message,
        const set05::ex39::PublicKey &public_key,
        size_t block_size
    );
    bool has_pkcs1_v15_prefix(
        const bignum::BigUint &plaintext,
        size_t block_size
    );
    bool padding_oracle(
        const bignum::BigUint &ciphertext,
        const set05::ex39::PrivateKey &private_key,
        size_t block_size
    );
    bignum::BigUint recover_plaintext_simple(
        const bignum::BigUint &ciphertext,
        const set05::ex39::PublicKey &public_key,
        const PaddingOracle &oracle,
        size_t query_limit = 1000000
    );

} // namespace set06::ex47
