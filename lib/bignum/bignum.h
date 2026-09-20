#pragma once

#include <cstddef>
#include <cstdint>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace bignum
{

    class BigUint
    {
    public:
        BigUint();
        BigUint(uint64_t value);

        static BigUint from_bytes_be(const std::vector<unsigned char> &bytes);
        static BigUint from_hex(const std::string &hex);

        std::vector<unsigned char> to_bytes_be() const;
        std::string to_hex() const;

        bool is_zero() const;
        bool is_odd() const;
        size_t bit_length() const;
        bool bit(size_t index) const;

        BigUint &operator+=(const BigUint &other);
        BigUint &operator-=(const BigUint &other);
        BigUint &operator*=(const BigUint &other);
        BigUint &operator<<=(size_t bits);
        BigUint &operator>>=(size_t bits);

        friend bool operator==(const BigUint &left, const BigUint &right);
        friend bool operator!=(const BigUint &left, const BigUint &right);
        friend bool operator<(const BigUint &left, const BigUint &right);
        friend bool operator>(const BigUint &left, const BigUint &right);
        friend bool operator<=(const BigUint &left, const BigUint &right);
        friend bool operator>=(const BigUint &left, const BigUint &right);

    private:
        explicit BigUint(std::vector<uint32_t> limbs);

        void normalize();

        std::vector<uint32_t> limbs_;

        friend BigUint operator/(const BigUint &left, const BigUint &right);
        friend BigUint operator%(const BigUint &left, const BigUint &right);
        friend std::pair<BigUint, BigUint> divide(
            const BigUint &dividend,
            const BigUint &divisor
        );
        friend BigUint random_bits(size_t bits, std::mt19937_64 &generator);
    };

    BigUint operator+(BigUint left, const BigUint &right);
    BigUint operator-(BigUint left, const BigUint &right);
    BigUint operator*(BigUint left, const BigUint &right);
    BigUint operator/(const BigUint &left, const BigUint &right);
    BigUint operator%(const BigUint &left, const BigUint &right);
    BigUint operator<<(BigUint value, size_t bits);
    BigUint operator>>(BigUint value, size_t bits);

    BigUint mod_pow(
        const BigUint &base,
        const BigUint &exponent,
        const BigUint &modulus
    );

    BigUint gcd(const BigUint &left, const BigUint &right);
    BigUint mod_inverse(const BigUint &value, const BigUint &modulus);

    BigUint integer_cube_root(const BigUint &value);

    BigUint random_bits(size_t bits, std::mt19937_64 &generator);
    BigUint random_below(const BigUint &limit, std::mt19937_64 &generator);
    bool is_probable_prime(const BigUint &value, size_t rounds = 16);
    BigUint random_prime(
        size_t bits,
        std::mt19937_64 &generator,
        size_t rounds = 16
    );

} // namespace bignum
