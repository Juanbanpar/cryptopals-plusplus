#include "ex37.h"

namespace set05::ex37
{

    AttackResult zero_key_attack(
        ex36::Server &server,
        const bignum::BigUint &transmitted_public_key
    )
    {
        const auto challenge = server.begin(transmitted_public_key);
        const auto forged_hmac = ex36::hmac_sha256(
            ex36::derive_session_key(bignum::BigUint(0)),
            challenge.salt
        );
        return {
            transmitted_public_key,
            forged_hmac,
            server.verify(transmitted_public_key, forged_hmac)};
    }

} // namespace set05::ex37
