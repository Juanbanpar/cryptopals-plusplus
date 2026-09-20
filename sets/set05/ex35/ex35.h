#pragma once

#include "ex33.h"
#include "ex34.h"

namespace set05::ex35
{

    struct AttackResult
    {
        bignum::BigUint malicious_generator;
        bignum::BigUint alice_public_key;
        bignum::BigUint bob_public_key;
        bignum::BigUint predictable_secret;
        ex34::EncryptedMessage alice_to_bob;
        ex34::EncryptedMessage bob_to_alice;
        std::vector<unsigned char> decrypted_alice_message;
        std::vector<unsigned char> decrypted_bob_message;
    };

    AttackResult malicious_generator_attack(
        const std::vector<unsigned char> &message,
        const ex33::Parameters &parameters,
        const bignum::BigUint &malicious_generator,
        const std::vector<unsigned char> &alice_iv = {},
        const std::vector<unsigned char> &bob_iv = {}
    );

} // namespace set05::ex35
