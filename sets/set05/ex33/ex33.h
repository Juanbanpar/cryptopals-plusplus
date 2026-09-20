#pragma once

#include "bignum.h"
#include <cstdint>

namespace set05::ex33
{

    using BigUint = bignum::BigUint;

    struct Parameters
    {
        BigUint p;
        BigUint g;
    };

    struct KeyPair
    {
        BigUint private_key;
        BigUint public_key;
    };

    Parameters toy_parameters();
    Parameters default_parameters();

    KeyPair generate_key_pair(const Parameters &parameters, uint64_t private_key);
    KeyPair generate_key_pair(const Parameters &parameters);

    BigUint derive_shared_secret(
        const Parameters &parameters,
        const BigUint &private_key,
        const BigUint &peer_public_key
    );

} // namespace set05::ex33
