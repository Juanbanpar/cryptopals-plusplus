#pragma once

#include "dsa.h"

#include <istream>

namespace set06::ex44
{

    struct SignedMessage
    {
        std::string message;
        dsa::Signature signature;
    };

    struct Recovery
    {
        bignum::BigUint private_key;
        bignum::BigUint nonce;
        size_t first_signature;
        size_t second_signature;
        bool found;
    };

    std::vector<SignedMessage> load_signatures(std::istream &input);
    dsa::PublicKey challenge_public_key();
    Recovery recover_reused_nonce(
        const std::vector<SignedMessage> &messages,
        const dsa::PublicKey &public_key
    );

} // namespace set06::ex44
