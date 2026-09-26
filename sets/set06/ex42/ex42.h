#pragma once

#include "ex39.h"

#include <vector>

namespace set06::ex42
{

    bignum::BigUint forge_signature(
        const std::vector<unsigned char> &message,
        const set05::ex39::PublicKey &public_key,
        size_t block_size
    );

    bool verify_signature_weakly(
        const std::vector<unsigned char> &message,
        const bignum::BigUint &signature,
        const set05::ex39::PublicKey &public_key,
        size_t block_size
    );

} // namespace set06::ex42
