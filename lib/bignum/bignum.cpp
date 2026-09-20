#include "bignum.h"

#include <algorithm>
#include <array>
#include <limits>
#include <stdexcept>
#include <utility>

namespace bignum
{

    namespace
    {
        constexpr uint64_t LIMB_BASE = uint64_t{1} << 32;
        constexpr uint64_t LIMB_MASK = LIMB_BASE - 1;

        std::vector<uint32_t> shift_left_limbs(
            const std::vector<uint32_t> &limbs,
            unsigned int bits
        )
        {
            if (bits == 0 || limbs.empty())
                return limbs;

            std::vector<uint32_t> result;
            result.reserve(limbs.size() + 1);
            uint64_t carry = 0;
            for (uint32_t limb : limbs)
            {
                const uint64_t value = (uint64_t{limb} << bits) | carry;
                result.push_back(static_cast<uint32_t>(value & LIMB_MASK));
                carry = value >> 32;
            }
            if (carry != 0)
                result.push_back(static_cast<uint32_t>(carry));
            return result;
        }

        std::vector<uint32_t> shift_right_limbs(
            const std::vector<uint32_t> &limbs,
            unsigned int bits
        )
        {
            if (bits == 0 || limbs.empty())
                return limbs;

            std::vector<uint32_t> result(limbs.size(), 0);
            uint32_t carry = 0;
            const uint32_t carry_mask = static_cast<uint32_t>((uint64_t{1} << bits) - 1);
            for (size_t i = limbs.size(); i-- > 0;)
            {
                const uint32_t limb = limbs[i];
                result[i] = (limb >> bits) | (uint32_t{carry} << (32 - bits));
                carry = limb & carry_mask;
            }
            while (result.size() > 1 && result.back() == 0)
                result.pop_back();
            return result;
        }

    }

    std::pair<BigUint, BigUint> divide(
            const BigUint &dividend,
            const BigUint &divisor
        )
        {
            if (divisor.is_zero())
                throw std::invalid_argument("division by zero");
            if (dividend < divisor)
                return {BigUint(0), dividend};

            if (divisor.limbs_.size() == 1)
            {
                const uint64_t divisor_limb = divisor.limbs_[0];
                std::vector<uint32_t> quotient(dividend.limbs_.size(), 0);
                uint64_t remainder = 0;
                for (size_t i = dividend.limbs_.size(); i-- > 0;)
                {
                    const uint64_t current = (remainder << 32) | dividend.limbs_[i];
                    quotient[i] = static_cast<uint32_t>(current / divisor_limb);
                    remainder = current % divisor_limb;
                }
                return {
                    BigUint(std::move(quotient)),
                    BigUint(remainder)
                };
            }

            const unsigned int normalization = static_cast<unsigned int>(
                __builtin_clz(divisor.limbs_.back())
            );
            const auto normalized_divisor = shift_left_limbs(divisor.limbs_, normalization);
            auto normalized_dividend = shift_left_limbs(dividend.limbs_, normalization);
            normalized_dividend.push_back(0);

            const size_t divisor_size = normalized_divisor.size();
            const size_t quotient_size = normalized_dividend.size() - divisor_size - 1;
            std::vector<uint32_t> quotient(quotient_size + 1, 0);

            for (size_t j = quotient_size + 1; j-- > 0;)
            {
                const uint64_t high_pair =
                    (uint64_t{normalized_dividend[j + divisor_size]} << 32) |
                    normalized_dividend[j + divisor_size - 1];
                uint64_t estimate = high_pair / normalized_divisor[divisor_size - 1];
                uint64_t estimate_remainder = high_pair % normalized_divisor[divisor_size - 1];

                if (estimate >= LIMB_BASE)
                {
                    estimate = LIMB_BASE - 1;
                    estimate_remainder += normalized_divisor[divisor_size - 1];
                }

                while (estimate_remainder < LIMB_BASE &&
                       estimate * normalized_divisor[divisor_size - 2] >
                           (estimate_remainder << 32) + normalized_dividend[j + divisor_size - 2])
                {
                    --estimate;
                    estimate_remainder += normalized_divisor[divisor_size - 1];
                }

                uint64_t carry = 0;
                uint64_t borrow = 0;
                for (size_t i = 0; i < divisor_size; ++i)
                {
                    const uint64_t product =
                        estimate * normalized_divisor[i] + carry;
                    carry = product >> 32;
                    const uint64_t subtrahend = (product & LIMB_MASK) + borrow;
                    const uint64_t current = normalized_dividend[j + i];
                    normalized_dividend[j + i] = static_cast<uint32_t>(current - subtrahend);
                    borrow = current < subtrahend ? 1 : 0;
                }

                const uint64_t high = normalized_dividend[j + divisor_size];
                const uint64_t high_subtrahend = carry + borrow;
                normalized_dividend[j + divisor_size] =
                    static_cast<uint32_t>(high - high_subtrahend);

                if (high < high_subtrahend)
                {
                    --estimate;
                    carry = 0;
                    for (size_t i = 0; i < divisor_size; ++i)
                    {
                        const uint64_t sum =
                            uint64_t{normalized_dividend[j + i]} + normalized_divisor[i] + carry;
                        normalized_dividend[j + i] = static_cast<uint32_t>(sum & LIMB_MASK);
                        carry = sum >> 32;
                    }
                    normalized_dividend[j + divisor_size] = static_cast<uint32_t>(
                        uint64_t{normalized_dividend[j + divisor_size]} + carry
                    );
                }

                quotient[j] = static_cast<uint32_t>(estimate);
            }

            std::vector<uint32_t> remainder_limbs(
                normalized_dividend.begin(),
                normalized_dividend.begin() + divisor_size
            );
            if (normalization != 0)
                remainder_limbs = shift_right_limbs(remainder_limbs, normalization);
            return {
                BigUint(std::move(quotient)),
                BigUint(std::move(remainder_limbs))
            };
        }

    namespace
    {

        BigUint positive_mod_subtract(
            const BigUint &left,
            const BigUint &right,
            const BigUint &modulus
        )
        {
            const BigUint reduced_left = left % modulus;
            const BigUint reduced_right = right % modulus;
            if (reduced_left >= reduced_right)
                return reduced_left - reduced_right;
            return modulus - (reduced_right - reduced_left);
        }

        struct SignedValue
        {
            bool negative = false;
            BigUint magnitude;
        };

        SignedValue signed_subtract(
            const SignedValue &left,
            const SignedValue &right
        )
        {
            if (left.negative != right.negative)
            {
                SignedValue result;
                result.negative = left.negative;
                result.magnitude = left.magnitude + right.magnitude;
                return result;
            }

            if (left.magnitude >= right.magnitude)
            {
                SignedValue result;
                result.negative = left.negative;
                result.magnitude = left.magnitude - right.magnitude;
                if (result.magnitude.is_zero())
                    result.negative = false;
                return result;
            }

            SignedValue result;
            result.negative = !left.negative;
            result.magnitude = right.magnitude - left.magnitude;
            return result;
        }

        BigUint absolute_mod(
            const SignedValue &value,
            const BigUint &modulus
        )
        {
            const BigUint reduced = value.magnitude % modulus;
            if (!value.negative || reduced.is_zero())
                return reduced;
            return modulus - reduced;
        }
    }

    BigUint::BigUint() : limbs_{0}
    {
    }

    BigUint::BigUint(uint64_t value)
    {
        if (value == 0)
        {
            limbs_.push_back(0);
            return;
        }
        while (value != 0)
        {
            limbs_.push_back(static_cast<uint32_t>(value & LIMB_MASK));
            value >>= 32;
        }
    }

    BigUint::BigUint(std::vector<uint32_t> limbs) : limbs_(std::move(limbs))
    {
        normalize();
    }

    void BigUint::normalize()
    {
        while (limbs_.size() > 1 && limbs_.back() == 0)
            limbs_.pop_back();
        if (limbs_.empty())
            limbs_.push_back(0);
    }

    BigUint BigUint::from_bytes_be(const std::vector<unsigned char> &bytes)
    {
        BigUint result;
        for (unsigned char byte : bytes)
        {
            result *= BigUint(256);
            result += BigUint(byte);
        }
        return result;
    }

    BigUint BigUint::from_hex(const std::string &hex)
    {
        size_t offset = 0;
        if (hex.size() >= 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))
            offset = 2;

        BigUint result;
        for (; offset < hex.size(); ++offset)
        {
            const char digit = hex[offset];
            unsigned int value = 0;
            if (digit >= '0' && digit <= '9')
                value = static_cast<unsigned int>(digit - '0');
            else if (digit >= 'a' && digit <= 'f')
                value = static_cast<unsigned int>(digit - 'a' + 10);
            else if (digit >= 'A' && digit <= 'F')
                value = static_cast<unsigned int>(digit - 'A' + 10);
            else
                throw std::invalid_argument("invalid hexadecimal digit");
            result *= BigUint(16);
            result += BigUint(value);
        }
        return result;
    }

    std::vector<unsigned char> BigUint::to_bytes_be() const
    {
        if (is_zero())
            return {0};

        std::vector<unsigned char> result;
        result.reserve((bit_length() + 7) / 8);
        for (size_t i = limbs_.size(); i-- > 0;)
        {
            const uint32_t limb = limbs_[i];
            for (int shift = 24; shift >= 0; shift -= 8)
            {
                const unsigned char byte = static_cast<unsigned char>(limb >> shift);
                if (!result.empty() || byte != 0)
                    result.push_back(byte);
            }
        }
        return result;
    }

    std::string BigUint::to_hex() const
    {
        static const char digits[] = "0123456789abcdef";
        const auto bytes = to_bytes_be();
        std::string result;
        result.reserve(bytes.size() * 2);
        for (unsigned char byte : bytes)
        {
            result.push_back(digits[byte >> 4]);
            result.push_back(digits[byte & 0x0f]);
        }
        const size_t first_nonzero = result.find_first_not_of('0');
        if (first_nonzero == std::string::npos)
            return "0";
        return result.substr(first_nonzero);
    }

    bool BigUint::is_zero() const
    {
        return limbs_.size() == 1 && limbs_[0] == 0;
    }

    bool BigUint::is_odd() const
    {
        return (limbs_[0] & 1) != 0;
    }

    size_t BigUint::bit_length() const
    {
        if (is_zero())
            return 0;
        return (limbs_.size() - 1) * 32 +
               (32 - static_cast<size_t>(__builtin_clz(limbs_.back())));
    }

    bool BigUint::bit(size_t index) const
    {
        const size_t limb = index / 32;
        if (limb >= limbs_.size())
            return false;
        return (limbs_[limb] & (uint32_t{1} << (index % 32))) != 0;
    }

    BigUint &BigUint::operator+=(const BigUint &other)
    {
        const size_t size = std::max(limbs_.size(), other.limbs_.size());
        limbs_.resize(size, 0);
        uint64_t carry = 0;
        for (size_t i = 0; i < size; ++i)
        {
            const uint64_t sum =
                uint64_t{limbs_[i]} + (i < other.limbs_.size() ? other.limbs_[i] : 0) + carry;
            limbs_[i] = static_cast<uint32_t>(sum & LIMB_MASK);
            carry = sum >> 32;
        }
        if (carry != 0)
            limbs_.push_back(static_cast<uint32_t>(carry));
        return *this;
    }

    BigUint &BigUint::operator-=(const BigUint &other)
    {
        if (*this < other)
            throw std::invalid_argument("unsigned subtraction would be negative");

        uint64_t borrow = 0;
        for (size_t i = 0; i < limbs_.size(); ++i)
        {
            const uint64_t subtrahend =
                uint64_t{borrow} + (i < other.limbs_.size() ? other.limbs_[i] : 0);
            const uint64_t current = limbs_[i];
            limbs_[i] = static_cast<uint32_t>(current - subtrahend);
            borrow = current < subtrahend ? 1 : 0;
        }
        normalize();
        return *this;
    }

    BigUint &BigUint::operator*=(const BigUint &other)
    {
        if (is_zero() || other.is_zero())
        {
            limbs_ = {0};
            return *this;
        }

        std::vector<uint32_t> result(limbs_.size() + other.limbs_.size(), 0);
        for (size_t i = 0; i < limbs_.size(); ++i)
        {
            uint64_t carry = 0;
            for (size_t j = 0; j < other.limbs_.size(); ++j)
            {
                const uint64_t current =
                    uint64_t{result[i + j]} +
                    uint64_t{limbs_[i]} * other.limbs_[j] +
                    carry;
                result[i + j] = static_cast<uint32_t>(current & LIMB_MASK);
                carry = current >> 32;
            }

            size_t index = i + other.limbs_.size();
            while (carry != 0)
            {
                const uint64_t current = uint64_t{result[index]} + carry;
                result[index] = static_cast<uint32_t>(current & LIMB_MASK);
                carry = current >> 32;
                ++index;
            }
        }
        limbs_ = std::move(result);
        normalize();
        return *this;
    }

    BigUint &BigUint::operator<<=(size_t bits)
    {
        if (is_zero() || bits == 0)
            return *this;

        const size_t whole_limbs = bits / 32;
        const unsigned int partial = static_cast<unsigned int>(bits % 32);
        std::vector<uint32_t> result(whole_limbs, 0);
        const auto shifted = shift_left_limbs(limbs_, partial);
        result.insert(result.end(), shifted.begin(), shifted.end());
        limbs_ = std::move(result);
        normalize();
        return *this;
    }

    BigUint &BigUint::operator>>=(size_t bits)
    {
        if (bits == 0)
            return *this;
        const size_t whole_limbs = bits / 32;
        if (whole_limbs >= limbs_.size())
        {
            limbs_ = {0};
            return *this;
        }

        limbs_.erase(limbs_.begin(), limbs_.begin() + whole_limbs);
        const unsigned int partial = static_cast<unsigned int>(bits % 32);
        if (partial != 0)
            limbs_ = shift_right_limbs(limbs_, partial);
        normalize();
        return *this;
    }

    bool operator==(const BigUint &left, const BigUint &right)
    {
        return left.limbs_ == right.limbs_;
    }

    bool operator!=(const BigUint &left, const BigUint &right)
    {
        return !(left == right);
    }

    bool operator<(const BigUint &left, const BigUint &right)
    {
        if (left.limbs_.size() != right.limbs_.size())
            return left.limbs_.size() < right.limbs_.size();
        for (size_t i = left.limbs_.size(); i-- > 0;)
        {
            if (left.limbs_[i] != right.limbs_[i])
                return left.limbs_[i] < right.limbs_[i];
        }
        return false;
    }

    bool operator>(const BigUint &left, const BigUint &right)
    {
        return right < left;
    }

    bool operator<=(const BigUint &left, const BigUint &right)
    {
        return !(right < left);
    }

    bool operator>=(const BigUint &left, const BigUint &right)
    {
        return !(left < right);
    }

    BigUint operator+(BigUint left, const BigUint &right)
    {
        left += right;
        return left;
    }

    BigUint operator-(BigUint left, const BigUint &right)
    {
        left -= right;
        return left;
    }

    BigUint operator*(BigUint left, const BigUint &right)
    {
        left *= right;
        return left;
    }

    BigUint operator/(const BigUint &left, const BigUint &right)
    {
        return divide(left, right).first;
    }

    BigUint operator%(const BigUint &left, const BigUint &right)
    {
        return divide(left, right).second;
    }

    BigUint operator<<(BigUint value, size_t bits)
    {
        value <<= bits;
        return value;
    }

    BigUint operator>>(BigUint value, size_t bits)
    {
        value >>= bits;
        return value;
    }

    BigUint mod_pow(
        const BigUint &base,
        const BigUint &exponent,
        const BigUint &modulus
    )
    {
        if (modulus.is_zero())
            throw std::invalid_argument("modulus must not be zero");

        BigUint result = BigUint(1) % modulus;
        BigUint current = base % modulus;
        BigUint power = exponent;
        while (!power.is_zero())
        {
            if (power.is_odd())
                result = (result * current) % modulus;
            power >>= 1;
            if (!power.is_zero())
                current = (current * current) % modulus;
        }
        return result;
    }

    BigUint gcd(const BigUint &left, const BigUint &right)
    {
        BigUint a = left;
        BigUint b = right;
        while (!b.is_zero())
        {
            BigUint remainder = a % b;
            a = std::move(b);
            b = std::move(remainder);
        }
        return a;
    }

    BigUint mod_inverse(const BigUint &value, const BigUint &modulus)
    {
        if (modulus <= BigUint(1))
            throw std::invalid_argument("modulus must be greater than one");

        BigUint remainder = value % modulus;
        BigUint old_remainder = modulus;
        SignedValue coefficient{false, BigUint(0)};
        SignedValue old_coefficient{false, BigUint(1)};

        while (!remainder.is_zero())
        {
            const BigUint quotient = old_remainder / remainder;
            const BigUint next_remainder = old_remainder % remainder;
            old_remainder = std::move(remainder);
            remainder = std::move(next_remainder);

            const SignedValue quotient_coefficient{
                old_coefficient.negative,
                quotient * old_coefficient.magnitude
            };
            const SignedValue next_coefficient = signed_subtract(
                coefficient,
                quotient_coefficient
            );
            coefficient = std::move(old_coefficient);
            old_coefficient = std::move(next_coefficient);
        }

        if (old_remainder != BigUint(1))
            throw std::invalid_argument("value has no modular inverse");
        return absolute_mod(coefficient, modulus);
    }

    BigUint integer_cube_root(const BigUint &value)
    {
        if (value.is_zero())
            return BigUint(0);

        BigUint low(0);
        BigUint high = BigUint(1) << ((value.bit_length() + 2) / 3 + 1);
        while (low + BigUint(1) < high)
        {
            const BigUint middle = (low + high) >> 1;
            const BigUint cube = middle * middle * middle;
            if (cube <= value)
                low = middle;
            else
                high = middle;
        }
        return low;
    }

    BigUint random_bits(size_t bits, std::mt19937_64 &generator)
    {
        if (bits == 0)
            return BigUint(0);

        const size_t limb_count = (bits + 31) / 32;
        std::vector<uint32_t> limbs(limb_count, 0);
        for (uint32_t &limb : limbs)
            limb = static_cast<uint32_t>(generator());

        const unsigned int excess = static_cast<unsigned int>(limb_count * 32 - bits);
        if (excess != 0)
            limbs.back() &= static_cast<uint32_t>(LIMB_MASK >> excess);
        return BigUint(std::move(limbs));
    }

    BigUint random_below(const BigUint &limit, std::mt19937_64 &generator)
    {
        if (limit.is_zero())
            throw std::invalid_argument("random limit must be greater than zero");
        BigUint result;
        do
        {
            result = random_bits(limit.bit_length(), generator);
        } while (result >= limit);
        return result;
    }

    bool is_probable_prime(const BigUint &value, size_t rounds)
    {
        if (value < BigUint(2))
            return false;

        static constexpr std::array<uint32_t, 16> SMALL_PRIMES = {
            2, 3, 5, 7, 11, 13, 17, 19,
            23, 29, 31, 37, 41, 43, 47, 53};
        for (uint32_t prime : SMALL_PRIMES)
        {
            if (value == BigUint(prime))
                return true;
            if ((value % BigUint(prime)).is_zero())
                return false;
        }

        BigUint d = value - BigUint(1);
        size_t powers_of_two = 0;
        while (!d.is_odd())
        {
            d >>= 1;
            ++powers_of_two;
        }

        static constexpr std::array<uint32_t, 16> BASES = {
            2, 3, 5, 7, 11, 13, 17, 19,
            23, 29, 31, 37, 41, 43, 47, 53};
        const size_t test_count = std::max<size_t>(1, std::min(rounds, BASES.size()));
        const BigUint n_minus_one = value - BigUint(1);
        for (size_t i = 0; i < test_count; ++i)
        {
            const BigUint base = BigUint(BASES[i]) % value;
            if (base.is_zero())
                continue;
            BigUint x = mod_pow(base, d, value);
            if (x == BigUint(1) || x == n_minus_one)
                continue;

            bool probably_prime = false;
            for (size_t r = 1; r < powers_of_two; ++r)
            {
                x = (x * x) % value;
                if (x == n_minus_one)
                {
                    probably_prime = true;
                    break;
                }
                if (x == BigUint(1))
                    return false;
            }
            if (!probably_prime)
                return false;
        }
        return true;
    }

    BigUint random_prime(
        size_t bits,
        std::mt19937_64 &generator,
        size_t rounds
    )
    {
        if (bits < 2)
            throw std::invalid_argument("prime size must be at least two bits");

        for (;;)
        {
            BigUint candidate = random_bits(bits, generator);
            if (!candidate.is_odd())
                candidate += BigUint(1);
            const BigUint high_bit = BigUint(1) << (bits - 1);
            if (candidate < high_bit)
                candidate += high_bit;
            if (is_probable_prime(candidate, rounds))
                return candidate;
        }
    }

} // namespace bignum
