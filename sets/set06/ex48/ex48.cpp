#include "ex48.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

namespace set06::ex48
{

    namespace
    {
        struct Interval
        {
            bignum::BigUint lower;
            bignum::BigUint upper;
        };

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

        bignum::BigUint multiply_ciphertext(
            const bignum::BigUint &ciphertext,
            const bignum::BigUint &multiplier,
            const set05::ex39::PublicKey &public_key
        )
        {
            return (ciphertext * bignum::mod_pow(
                                   multiplier,
                                   public_key.e,
                                   public_key.n
                               )) % public_key.n;
        }

        std::vector<Interval> narrow_intervals(
            const std::vector<Interval> &intervals,
            const bignum::BigUint &multiplier,
            const bignum::BigUint &modulus,
            const bignum::BigUint &two_b,
            const bignum::BigUint &three_b_minus_one
        )
        {
            std::vector<Interval> narrowed;
            for (const auto &interval : intervals)
            {
                const auto low_product = interval.lower * multiplier;
                const auto high_product = interval.upper * multiplier;
                const auto first_r = low_product > three_b_minus_one
                                         ? ceil_div(low_product - three_b_minus_one, modulus)
                                         : bignum::BigUint(0);
                if (high_product < two_b)
                    continue;
                const auto last_r = (high_product - two_b) / modulus;
                for (auto r = first_r; r <= last_r; r += bignum::BigUint(1))
                {
                    const auto candidate_lower = ceil_div(
                        two_b + r * modulus, multiplier
                    );
                    const auto candidate_upper =
                        (three_b_minus_one + r * modulus) / multiplier;
                    const auto lower = candidate_lower > interval.lower
                                           ? candidate_lower
                                           : interval.lower;
                    const auto upper = candidate_upper < interval.upper
                                           ? candidate_upper
                                           : interval.upper;
                    if (lower <= upper)
                        narrowed.push_back({lower, upper});
                }
            }

            std::sort(
                narrowed.begin(), narrowed.end(),
                [](const Interval &left, const Interval &right) {
                    return left.lower < right.lower;
                }
            );
            std::vector<Interval> merged;
            for (const auto &interval : narrowed)
            {
                if (merged.empty() || interval.lower > merged.back().upper + bignum::BigUint(1))
                    merged.push_back(interval);
                else if (interval.upper > merged.back().upper)
                    merged.back().upper = interval.upper;
            }
            return merged;
        }
    }

    bignum::BigUint recover_plaintext_complete(
        const bignum::BigUint &ciphertext,
        const set05::ex39::PublicKey &public_key,
        const ex47::PaddingOracle &oracle,
        size_t query_limit
    )
    {
        if (!oracle || ciphertext >= public_key.n)
            throw std::invalid_argument("invalid ciphertext or padding oracle");
        if (!oracle(ciphertext))
            throw std::invalid_argument("initial ciphertext is not PKCS#1 v1.5 conformant");

        const size_t block_size = (public_key.n.bit_length() + 7) / 8;
        if (block_size < 11)
            throw std::invalid_argument("RSA modulus is too short for PKCS#1 v1.5");
        const auto b = bignum::BigUint(1) << (8 * block_size - 16);
        const auto two_b = b * bignum::BigUint(2);
        const auto three_b_minus_one = b * bignum::BigUint(3) - bignum::BigUint(1);
        const auto &modulus = public_key.n;
        std::vector<Interval> intervals{{two_b, three_b_minus_one}};
        size_t queries = 1;

        auto multiplier = ceil_div(modulus, b * bignum::BigUint(3));
        for (;;)
        {
            if (++queries > query_limit)
                throw std::runtime_error("padding-oracle query limit exceeded");
            if (oracle(multiply_ciphertext(ciphertext, multiplier, public_key)))
                break;
            multiplier += bignum::BigUint(1);
        }

        for (size_t iteration = 0; iteration < modulus.bit_length() + 16; ++iteration)
        {
            intervals = narrow_intervals(
                intervals,
                multiplier,
                modulus,
                two_b,
                three_b_minus_one
            );
            if (intervals.empty())
                throw std::runtime_error("padding-oracle attack eliminated all plaintext intervals");
            if (intervals.size() == 1 && intervals[0].lower == intervals[0].upper)
                return intervals[0].lower;

            if (intervals.size() > 1)
            {
                for (multiplier += bignum::BigUint(1);; multiplier += bignum::BigUint(1))
                {
                    if (++queries > query_limit)
                        throw std::runtime_error("padding-oracle query limit exceeded");
                    if (oracle(multiply_ciphertext(ciphertext, multiplier, public_key)))
                        break;
                }
                continue;
            }

            const auto &interval = intervals.front();
            if (interval.upper * multiplier < two_b)
                throw std::runtime_error("padding-oracle interval became inconsistent");
            auto r = ceil_div(
                (interval.upper * multiplier - two_b) * bignum::BigUint(2),
                modulus
            );
            for (;;)
            {
                const auto candidate = ceil_div(
                    two_b + r * modulus,
                    interval.upper
                );
                if (++queries > query_limit)
                    throw std::runtime_error("padding-oracle query limit exceeded");
                if (oracle(multiply_ciphertext(ciphertext, candidate, public_key)))
                {
                    multiplier = candidate;
                    break;
                }
                r += bignum::BigUint(1);
            }
        }
        throw std::runtime_error("complete padding-oracle attack did not converge");
    }

} // namespace set06::ex48
