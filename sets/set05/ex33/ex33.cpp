#include "ex33.h"

#include <random>
#include <stdexcept>

namespace set05::ex33
{

    namespace
    {
        const char *NIST_PRIME =
            "ffffffffffffffffc90fdaa22168c234c4c6628b80dc1cd129024e"
            "088a67cc74020bbea63b139b22514a08798e3404ddef9519b3cd3a431b"
            "302b0a6df25f14374fe1356d6d51c245e485b576625e7ec6f44c42e9a637e"
            "d6b0bff5cb6f406b7edee386bfb5a899fa5ae9f24117c4b1fe649286651ec"
            "e45b3dc2007cb8a163bf0598da48361c55d39a69163fa8fd24cf5f83655d2"
            "3dca3ad961c62f356208552bb9ed529077096966d670c354e4abc9804f174"
            "6c08ca237327ffffffffffffffff";

        std::mt19937_64 &generator()
        {
            static std::random_device device;
            static std::mt19937_64 value(device());
            return value;
        }

        void validate_parameters(const Parameters &parameters)
        {
            if (parameters.p <= BigUint(2))
                throw std::invalid_argument("DH modulus must be greater than two");
            if (parameters.g.is_zero() || parameters.g >= parameters.p)
                throw std::invalid_argument("DH generator must be in the modulus range");
        }
    }

    Parameters toy_parameters()
    {
        return {BigUint(37), BigUint(5)};
    }

    Parameters default_parameters()
    {
        return {BigUint::from_hex(NIST_PRIME), BigUint(2)};
    }

    KeyPair generate_key_pair(const Parameters &parameters, uint64_t private_key)
    {
        validate_parameters(parameters);
        if (private_key == 0 || BigUint(private_key) >= parameters.p)
            throw std::invalid_argument("DH private key must be in the modulus range");
        const BigUint private_value(private_key);
        return {private_value, bignum::mod_pow(parameters.g, private_value, parameters.p)};
    }

    KeyPair generate_key_pair(const Parameters &parameters)
    {
        validate_parameters(parameters);
        const BigUint private_value =
            bignum::random_below(parameters.p - BigUint(2), generator()) + BigUint(2);
        return {private_value, bignum::mod_pow(parameters.g, private_value, parameters.p)};
    }

    BigUint derive_shared_secret(
        const Parameters &parameters,
        const BigUint &private_key,
        const BigUint &peer_public_key
    )
    {
        validate_parameters(parameters);
        if (private_key.is_zero() || private_key >= parameters.p)
            throw std::invalid_argument("DH private key must be in the modulus range");
        return bignum::mod_pow(peer_public_key, private_key, parameters.p);
    }

} // namespace set05::ex33
