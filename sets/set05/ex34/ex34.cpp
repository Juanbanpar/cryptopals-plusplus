#include "ex34.h"
#include "aes.h"
#include "common.h"
#include "sha1.h"

#include <stdexcept>

namespace set05::ex34
{

    namespace
    {
        std::vector<unsigned char> message_iv(const std::vector<unsigned char> &iv)
        {
            if (iv.empty())
                return rand_bytes(16);
            if (iv.size() != 16)
                throw std::invalid_argument("CBC IV must be 16 bytes");
            return iv;
        }
    }

    std::vector<unsigned char> derive_aes_key(const bignum::BigUint &shared_secret)
    {
        const auto digest = sha1::digest(shared_secret.to_bytes_be());
        return std::vector<unsigned char>(digest.begin(), digest.begin() + 16);
    }

    EncryptedMessage encrypt_message(
        const std::vector<unsigned char> &message,
        const bignum::BigUint &shared_secret,
        const std::vector<unsigned char> &iv
    )
    {
        const auto message_iv_value = message_iv(iv);
        return {
            aes::encrypt_cbc(message, derive_aes_key(shared_secret), message_iv_value),
            message_iv_value};
    }

    std::vector<unsigned char> decrypt_message(
        const EncryptedMessage &message,
        const bignum::BigUint &shared_secret
    )
    {
        if (message.iv.size() != 16)
            throw std::invalid_argument("CBC IV must be 16 bytes");
        return aes::decrypt_cbc(
            message.ciphertext,
            derive_aes_key(shared_secret),
            message.iv
        );
    }

    AttackResult parameter_injection_attack(
        const std::vector<unsigned char> &message,
        const ex33::Parameters &parameters,
        const std::vector<unsigned char> &alice_iv,
        const std::vector<unsigned char> &bob_iv
    )
    {
        const auto alice = ex33::generate_key_pair(parameters, 5);
        const auto bob = ex33::generate_key_pair(parameters, 7);

        // The MITM replaces both public keys with p. Both honest parties then
        // derive p**private_key mod p, which is the predictable value zero.
        const bignum::BigUint predictable_secret(0);
        const auto alice_secret = ex33::derive_shared_secret(
            parameters, alice.private_key, predictable_secret
        );
        const auto bob_secret = ex33::derive_shared_secret(
            parameters, bob.private_key, predictable_secret
        );
        if (!alice_secret.is_zero() || !bob_secret.is_zero())
            throw std::logic_error("DH parameter injection did not produce zero");

        const auto alice_to_bob = encrypt_message(message, alice_secret, alice_iv);
        const auto echoed_message = decrypt_message(alice_to_bob, bob_secret);
        const auto bob_to_alice = encrypt_message(echoed_message, bob_secret, bob_iv);

        return {
            predictable_secret,
            alice_to_bob,
            bob_to_alice,
            decrypt_message(alice_to_bob, predictable_secret),
            decrypt_message(bob_to_alice, predictable_secret)};
    }

} // namespace set05::ex34
