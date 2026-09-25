#include "dsa.h"

#include "sha1.h"

#include <random>
#include <stdexcept>

namespace set06::dsa
{

    namespace
    {
        std::mt19937_64 &generator()
        {
            static std::random_device device;
            static std::mt19937_64 value(device());
            return value;
        }

        std::string bytes_to_hex(const std::vector<unsigned char> &bytes)
        {
            static const char digits[] = "0123456789abcdef";
            std::string result;
            result.reserve(bytes.size() * 2);
            for (unsigned char byte : bytes)
            {
                result.push_back(digits[byte >> 4]);
                result.push_back(digits[byte & 0x0f]);
            }
            return result;
        }
    }

    Parameters challenge_parameters()
    {
        return {
            bignum::BigUint::from_hex(
                "800000000000000089e1855218a0e7dac38136ffafa72eda7859f2171e25e65e"
                "ac698c1702578b07dc2a1076da241c76c62d374d8389ea5aeffd3226a0530cc5"
                "65f3bf6b50929139ebeac04f48c3c84afb796d61e5a4f9a8fda812ab59494232c"
                "7d2b4deb50aa18ee9e132bfa85ac4374d7f9091abc3d015efc871a584471bb1"),
            bignum::BigUint::from_hex("f4f47f05794b256174bba6e9b396a7707e563c5b"),
            bignum::BigUint::from_hex(
                "5958c9d3898b224b12672c0b98e06c60df923cb8bc999d119458fef538b8fa404"
                "6c8db53039db620c094c9fa077ef389b5322a559946a71903f990f1f7e0e025e"
                "2d7f7cf494aff1a0470f5b64c36b625a097f1651fe775323556fe00b3608c887892"
                "878480e99041be601a62166ca6894bdd41a7054ec89f756ba9fc95302291")};
    }

    KeyPair key_pair_from_private(const Parameters &parameters, const bignum::BigUint &x)
    {
        if (parameters.p <= bignum::BigUint(2) ||
            parameters.q <= bignum::BigUint(1) ||
            x.is_zero() || x >= parameters.q)
            throw std::invalid_argument("invalid DSA parameters or private key");
        return {
            {parameters, bignum::mod_pow(parameters.g, x, parameters.p)},
            {parameters, x}};
    }

    KeyPair generate_key_pair(const Parameters &parameters)
    {
        const auto limit = parameters.q - bignum::BigUint(1);
        const auto x = bignum::random_below(limit, generator()) + bignum::BigUint(1);
        return key_pair_from_private(parameters, x);
    }

    bignum::BigUint message_hash(const std::vector<unsigned char> &message)
    {
        return bignum::BigUint::from_bytes_be(sha1::digest(message));
    }

    bignum::BigUint message_hash(const std::string &message)
    {
        return message_hash(std::vector<unsigned char>(message.begin(), message.end()));
    }

    bignum::BigUint subtract_mod(
        const bignum::BigUint &left,
        const bignum::BigUint &right,
        const bignum::BigUint &modulus
    )
    {
        const auto reduced_left = left % modulus;
        const auto reduced_right = right % modulus;
        if (reduced_left >= reduced_right)
            return reduced_left - reduced_right;
        return modulus - (reduced_right - reduced_left);
    }

    Signature sign(
        const std::vector<unsigned char> &message,
        const PrivateKey &private_key
    )
    {
        const auto limit = private_key.parameters.q - bignum::BigUint(1);
        const auto nonce = bignum::random_below(limit, generator()) + bignum::BigUint(1);
        return sign(message, private_key, nonce);
    }

    Signature sign(
        const std::vector<unsigned char> &message,
        const PrivateKey &private_key,
        const bignum::BigUint &nonce
    )
    {
        const auto &parameters = private_key.parameters;
        if (nonce.is_zero() || nonce >= parameters.q)
            throw std::invalid_argument("DSA nonce must be in the range [1, q-1]");
        const auto r = bignum::mod_pow(parameters.g, nonce, parameters.p) % parameters.q;
        const auto s = (bignum::mod_inverse(nonce, parameters.q) *
                        (message_hash(message) + private_key.x * r)) % parameters.q;
        return {r, s};
    }

    bool verify(
        const std::vector<unsigned char> &message,
        const Signature &signature,
        const PublicKey &public_key
    )
    {
        const auto &parameters = public_key.parameters;
        if (signature.r >= parameters.q || signature.s.is_zero() ||
            signature.s >= parameters.q)
            return false;

        const auto w = bignum::mod_inverse(signature.s, parameters.q);
        const auto u1 = (message_hash(message) * w) % parameters.q;
        const auto u2 = (signature.r * w) % parameters.q;
        const auto v = ((bignum::mod_pow(parameters.g, u1, parameters.p) *
                         bignum::mod_pow(public_key.y, u2, parameters.p)) %
                        parameters.p) % parameters.q;
        return v == signature.r;
    }

    std::string private_key_fingerprint(const bignum::BigUint &private_key)
    {
        const auto hex = private_key.to_hex();
        const std::vector<unsigned char> encoded(hex.begin(), hex.end());
        return bytes_to_hex(sha1::digest(encoded));
    }

} // namespace set06::dsa
