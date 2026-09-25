#pragma once

#include "ex47.h"

namespace set06::ex48
{

    bignum::BigUint recover_plaintext_complete(
        const bignum::BigUint &ciphertext,
        const set05::ex39::PublicKey &public_key,
        const ex47::PaddingOracle &oracle,
        size_t query_limit = 1000000
    );

} // namespace set06::ex48
