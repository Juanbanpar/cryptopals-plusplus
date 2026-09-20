#pragma once

#include "ex33.h"
#include <vector>

namespace set05::ex34
{

    struct EncryptedMessage
    {
        std::vector<unsigned char> ciphertext;
        std::vector<unsigned char> iv;
    };

    struct AttackResult
    {
        bignum::BigUint predictable_secret;
        EncryptedMessage alice_to_bob;
        EncryptedMessage bob_to_alice;
        std::vector<unsigned char> decrypted_alice_message;
        std::vector<unsigned char> decrypted_bob_message;
    };

    std::vector<unsigned char> derive_aes_key(const bignum::BigUint &shared_secret);

    EncryptedMessage encrypt_message(
        const std::vector<unsigned char> &message,
        const bignum::BigUint &shared_secret,
        const std::vector<unsigned char> &iv = {}
    );

    std::vector<unsigned char> decrypt_message(
        const EncryptedMessage &message,
        const bignum::BigUint &shared_secret
    );

    AttackResult parameter_injection_attack(
        const std::vector<unsigned char> &message,
        const ex33::Parameters &parameters = ex33::toy_parameters(),
        const std::vector<unsigned char> &alice_iv = {},
        const std::vector<unsigned char> &bob_iv = {}
    );

} // namespace set05::ex34
