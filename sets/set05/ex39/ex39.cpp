#include "ex39.h"

#include <random>
#include <stdexcept>

namespace set05::ex39
{

    namespace
    {
        std::mt19937_64 &generator()
        {
            static std::random_device device;
            static std::mt19937_64 value(device());
            return value;
        }
    }

    KeyPair key_pair_from_primes(const BigUint &p, const BigUint &q)
    {
        if (p <= BigUint(2) || q <= BigUint(2) || p == q)
            throw std::invalid_argument("RSA primes must be distinct and greater than two");
        if (!bignum::is_probable_prime(p) || !bignum::is_probable_prime(q))
            throw std::invalid_argument("RSA key inputs must be prime");

        const BigUint e(3);
        const BigUint n = p * q;
        const BigUint totient = (p - BigUint(1)) * (q - BigUint(1));
        if (bignum::gcd(e, totient) != BigUint(1))
            throw std::invalid_argument("RSA public exponent is not invertible");
        return {
            {e, n},
            {bignum::mod_inverse(e, totient), n},
            p,
            q};
    }

    KeyPair generate_key_pair(size_t prime_bits)
    {
        if (prime_bits < 4)
            throw std::invalid_argument("RSA prime size must be at least four bits");

        for (;;)
        {
            const BigUint p = bignum::random_prime(prime_bits, generator());
            const BigUint q = bignum::random_prime(prime_bits, generator());
            if (p == q)
                continue;
            try
            {
                return key_pair_from_primes(p, q);
            }
            catch (const std::invalid_argument &)
            {
                // Retry when one of the generated primes makes e=3
                // non-invertible modulo the totient.
            }
        }
    }

    BigUint encrypt(const BigUint &message, const PublicKey &key)
    {
        if (key.n <= BigUint(1))
            throw std::invalid_argument("RSA modulus must be greater than one");
        if (message >= key.n)
            throw std::invalid_argument("RSA message must be smaller than the modulus");
        return bignum::mod_pow(message, key.e, key.n);
    }

    BigUint decrypt(const BigUint &ciphertext, const PrivateKey &key)
    {
        if (key.n <= BigUint(1))
            throw std::invalid_argument("RSA modulus must be greater than one");
        if (ciphertext >= key.n)
            throw std::invalid_argument("RSA ciphertext must be smaller than the modulus");
        return bignum::mod_pow(ciphertext, key.d, key.n);
    }

    BigUint bytes_to_integer(const std::vector<unsigned char> &message)
    {
        return BigUint::from_bytes_be(message);
    }

    std::vector<unsigned char> integer_to_bytes(const BigUint &message, size_t width)
    {
        auto bytes = message.to_bytes_be();
        if (width == 0)
            return bytes;
        if (bytes.size() > width)
            throw std::invalid_argument("integer does not fit requested width");
        std::vector<unsigned char> result(width - bytes.size(), 0);
        result.insert(result.end(), bytes.begin(), bytes.end());
        return result;
    }

    BigUint encrypt_message(
        const std::vector<unsigned char> &message,
        const PublicKey &key
    )
    {
        return encrypt(bytes_to_integer(message), key);
    }

    std::vector<unsigned char> decrypt_message(
        const BigUint &ciphertext,
        const PrivateKey &key,
        size_t width
    )
    {
        return integer_to_bytes(decrypt(ciphertext, key), width);
    }

} // namespace set05::ex39
