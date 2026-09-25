#include "ex46.h"

#include <stdexcept>

namespace set06::ex46
{

    namespace
    {
        bignum::BigUint ceil_div(
            const bignum::BigUint &numerator,
            const bignum::BigUint &denominator
        )
        {
            auto quotient = numerator / denominator;
            if (!(numerator % denominator).is_zero())
                quotient += bignum::BigUint(1);
            return quotient;
        }
    }

    bignum::BigUint recover_plaintext(
        const bignum::BigUint &ciphertext,
        const set05::ex39::PublicKey &public_key,
        const ParityOracle &is_odd
    )
    {
        if (!is_odd || ciphertext >= public_key.n)
            throw std::invalid_argument("invalid ciphertext or parity oracle");

        const auto original = ciphertext;
        auto current = ciphertext;
        bignum::BigUint low(0);
        bignum::BigUint high = public_key.n;
        const auto twice = bignum::mod_pow(
            bignum::BigUint(2), public_key.e, public_key.n
        );
        const size_t iterations = public_key.n.bit_length() + 2;

        for (size_t i = 0; i < iterations; ++i)
        {
            current = (current * twice) % public_key.n;
            const auto midpoint = low + high;
            low <<= 1;
            high <<= 1;
            if (is_odd(current))
                low = midpoint;
            else
                high = midpoint;
        }

        const auto denominator = bignum::BigUint(1) << iterations;
        auto candidate = low / denominator;
        const auto last_candidate = ceil_div(high, denominator);
        for (size_t attempts = 0; candidate <= last_candidate && attempts < 3; ++attempts)
        {
            if (set05::ex39::encrypt(candidate, public_key) == original)
                return candidate;
            candidate += bignum::BigUint(1);
        }
        throw std::runtime_error("parity oracle interval did not identify a plaintext");
    }

} // namespace set06::ex46
