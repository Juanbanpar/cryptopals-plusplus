#pragma once

#include "ex39.h"

#include <functional>

namespace set06::ex46
{

    using ParityOracle = std::function<bool(const bignum::BigUint &)>;

    bignum::BigUint recover_plaintext(
        const bignum::BigUint &ciphertext,
        const set05::ex39::PublicKey &public_key,
        const ParityOracle &is_odd
    );

} // namespace set06::ex46
