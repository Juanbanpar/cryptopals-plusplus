#pragma once

#include "ex36.h"

namespace set05::ex37
{

    struct AttackResult
    {
        bignum::BigUint transmitted_public_key;
        std::vector<unsigned char> forged_hmac;
        bool accepted;
    };

    AttackResult zero_key_attack(
        ex36::Server &server,
        const bignum::BigUint &transmitted_public_key
    );

} // namespace set05::ex37
