#include "ex47.h"

#include <algorithm>
#include <random>
#include <stdexcept>

namespace set06::ex47
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

        bignum::BigUint padded_bound(size_t block_size)
        {
            if (block_size < 2)
                throw std::invalid_argument("RSA block size is too short");
            return bignum::BigUint(1) << (8 * block_size - 16);
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

        bool update_single_interval(
            bignum::BigUint &lower,
            bignum::BigUint &upper,
            const bignum::BigUint &multiplier,
            const bignum::BigUint &modulus,
            const bignum::BigUint &two_b,
            const bignum::BigUint &three_b_minus_one
        )
        {
            const auto low_product = lower * multiplier;
            const auto high_product = upper * multiplier;
            const auto first_r = low_product > three_b_minus_one
                                     ? ceil_div(low_product - three_b_minus_one, modulus)
                                     : bignum::BigUint(0);
            if (high_product < two_b)
                return false;
            const auto last_r = (high_product - two_b) / modulus;
            if (first_r != last_r)
                throw std::runtime_error("simple padding-oracle attack produced multiple ranges");

            const auto candidate_lower = ceil_div(
                two_b + first_r * modulus, multiplier
            );
            const auto candidate_upper =
                (three_b_minus_one + first_r * modulus) / multiplier;
            lower = candidate_lower > lower ? candidate_lower : lower;
            upper = candidate_upper < upper ? candidate_upper : upper;
            return lower <= upper;
        }
    }

    std::vector<unsigned char> pad_message(
        const std::vector<unsigned char> &message,
        size_t block_size
    )
    {
        if (block_size < 11 || message.size() > block_size - 11)
            throw std::invalid_argument("message does not fit PKCS#1 v1.5 block");
        const size_t delimiter = block_size - message.size() - 1;
        std::random_device device;
        std::uniform_int_distribution<unsigned int> byte_distribution(1, 255);
        std::vector<unsigned char> block(block_size, 0);
        block[1] = 0x02;
        for (size_t i = 2; i < delimiter; ++i)
            block[i] = static_cast<unsigned char>(byte_distribution(device));
        block[delimiter] = 0;
        std::copy(message.begin(), message.end(), block.begin() + delimiter + 1);
        return block;
    }

    bignum::BigUint encrypt_padded(
        const std::vector<unsigned char> &message,
        const set05::ex39::PublicKey &public_key,
        size_t block_size
    )
    {
        return set05::ex39::encrypt(
            bignum::BigUint::from_bytes_be(pad_message(message, block_size)),
            public_key
        );
    }

    bool has_pkcs1_v15_prefix(
        const bignum::BigUint &plaintext,
        size_t block_size
    )
    {
        const auto bytes = set05::ex39::integer_to_bytes(plaintext, block_size);
        return bytes.size() >= 2 && bytes[0] == 0 && bytes[1] == 2;
    }

    bool padding_oracle(
        const bignum::BigUint &ciphertext,
        const set05::ex39::PrivateKey &private_key,
        size_t block_size
    )
    {
        return has_pkcs1_v15_prefix(
            set05::ex39::decrypt(ciphertext, private_key), block_size
        );
    }

    bignum::BigUint recover_plaintext_simple(
        const bignum::BigUint &ciphertext,
        const set05::ex39::PublicKey &public_key,
        const PaddingOracle &oracle,
        size_t query_limit
    )
    {
        if (!oracle || ciphertext >= public_key.n)
            throw std::invalid_argument("invalid ciphertext or padding oracle");
        if (!oracle(ciphertext))
            throw std::invalid_argument("initial ciphertext is not PKCS#1 v1.5 conformant");

        const size_t block_size = (public_key.n.bit_length() + 7) / 8;
        const auto b = padded_bound(block_size);
        const auto two_b = b * bignum::BigUint(2);
        const auto three_b_minus_one = b * bignum::BigUint(3) - bignum::BigUint(1);
        auto lower = two_b;
        auto upper = three_b_minus_one;
        size_t queries = 1;

        auto multiplier = ceil_div(public_key.n, b * bignum::BigUint(3));
        for (;;)
        {
            if (++queries > query_limit)
                throw std::runtime_error("padding-oracle query limit exceeded");
            if (oracle(multiply_ciphertext(ciphertext, multiplier, public_key)))
                break;
            multiplier += bignum::BigUint(1);
        }

        for (size_t iteration = 0; iteration < public_key.n.bit_length() + 8; ++iteration)
        {
            if (!update_single_interval(
                    lower,
                    upper,
                    multiplier,
                    public_key.n,
                    two_b,
                    three_b_minus_one
                ))
                throw std::runtime_error("padding-oracle attack eliminated the plaintext interval");
            if (lower == upper)
                return lower;

            auto r = ceil_div(
                (upper * multiplier - two_b) * bignum::BigUint(2),
                public_key.n
            );
            for (;;)
            {
                const auto candidate = ceil_div(two_b + r * public_key.n, upper);
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
        throw std::runtime_error("padding-oracle attack did not converge");
    }

} // namespace set06::ex47
