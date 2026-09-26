#pragma once

#include "dsa.h"

namespace set06::ex45
{

    dsa::Signature forge_universal_signature(
        const dsa::PublicKey &public_key,
        const bignum::BigUint &z
    );

} // namespace set06::ex45
