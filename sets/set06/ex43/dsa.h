#pragma once

#include "bignum.h"

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace set06::dsa
{

    struct Parameters
    {
        bignum::BigUint p;
        bignum::BigUint q;
        bignum::BigUint g;
    };

    struct PublicKey
    {
        Parameters parameters;
        bignum::BigUint y;
    };

    struct PrivateKey
    {
        Parameters parameters;
        bignum::BigUint x;
    };

    struct KeyPair
    {
        PublicKey public_key;
        PrivateKey private_key;
    };

    struct Signature
    {
        bignum::BigUint r;
        bignum::BigUint s;
    };

    Parameters challenge_parameters();
    KeyPair key_pair_from_private(const Parameters &parameters, const bignum::BigUint &x);
    KeyPair generate_key_pair(const Parameters &parameters);

    Signature sign(
        const std::vector<unsigned char> &message,
        const PrivateKey &private_key
    );
    Signature sign(
        const std::vector<unsigned char> &message,
        const PrivateKey &private_key,
        const bignum::BigUint &nonce
    );
    bool verify(
        const std::vector<unsigned char> &message,
        const Signature &signature,
        const PublicKey &public_key
    );

    bignum::BigUint message_hash(const std::vector<unsigned char> &message);
    bignum::BigUint message_hash(const std::string &message);
    bignum::BigUint subtract_mod(
        const bignum::BigUint &left,
        const bignum::BigUint &right,
        const bignum::BigUint &modulus
    );
    std::string private_key_fingerprint(const bignum::BigUint &private_key);

} // namespace set06::dsa
