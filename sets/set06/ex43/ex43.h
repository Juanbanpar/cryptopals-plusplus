#pragma once

#include "dsa.h"

namespace set06::ex43
{

    struct Recovery
    {
        bignum::BigUint private_key;
        bignum::BigUint nonce;
        bool found;
    };

    Recovery recover_key_from_small_nonce(
        const std::vector<unsigned char> &message,
        const dsa::Signature &signature,
        const dsa::PublicKey &public_key,
        uint64_t nonce_limit = 65536
    );

    dsa::PublicKey challenge_public_key();
    dsa::Signature challenge_signature();
    std::vector<unsigned char> challenge_message();

} // namespace set06::ex43
