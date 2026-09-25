#include "ex45.h"

#include <stdexcept>

namespace set06::ex45
{

    dsa::Signature forge_universal_signature(
        const dsa::PublicKey &public_key,
        const bignum::BigUint &z
    )
    {
        const auto &parameters = public_key.parameters;
        if (z.is_zero() || z >= parameters.q)
            throw std::invalid_argument("z must be in the range [1, q-1]");
        const auto r = bignum::mod_pow(public_key.y, z, parameters.p) % parameters.q;
        if (r.is_zero())
            throw std::invalid_argument("z produced a zero DSA r value");
        const auto s = (r * bignum::mod_inverse(z, parameters.q)) % parameters.q;
        return {r, s};
    }

} // namespace set06::ex45
