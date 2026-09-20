#include "ex35.h"

#include <stdexcept>

namespace set05::ex35
{

    AttackResult malicious_generator_attack(
        const std::vector<unsigned char> &message,
        const ex33::Parameters &parameters,
        const bignum::BigUint &malicious_generator,
        const std::vector<unsigned char> &alice_iv,
        const std::vector<unsigned char> &bob_iv
    )
    {
        if (malicious_generator != bignum::BigUint(1) &&
            malicious_generator != parameters.p &&
            malicious_generator != parameters.p - bignum::BigUint(1))
            throw std::invalid_argument("unsupported malicious DH generator");

        const auto alice = ex33::generate_key_pair(parameters, 5);
        const auto bob = ex33::generate_key_pair(parameters, 7);
        const auto alice_public_key = bignum::mod_pow(
            malicious_generator, alice.private_key, parameters.p
        );
        const auto bob_public_key = bignum::mod_pow(
            malicious_generator, bob.private_key, parameters.p
        );

        bignum::BigUint predictable_secret;
        if (malicious_generator == bignum::BigUint(1))
            predictable_secret = bignum::BigUint(1);
        else if (malicious_generator == parameters.p)
            predictable_secret = bignum::BigUint(0);
        else
            predictable_secret =
                alice_public_key == parameters.p - bignum::BigUint(1) &&
                        bob_public_key == parameters.p - bignum::BigUint(1)
                    ? parameters.p - bignum::BigUint(1)
                    : bignum::BigUint(1);

        const auto alice_secret = ex33::derive_shared_secret(
            parameters, alice.private_key, bob_public_key
        );
        const auto bob_secret = ex33::derive_shared_secret(
            parameters, bob.private_key, alice_public_key
        );
        if (alice_secret != predictable_secret || bob_secret != predictable_secret)
            throw std::logic_error("malicious generator was not predictable");

        const auto alice_to_bob = ex34::encrypt_message(message, alice_secret, alice_iv);
        const auto echoed_message = ex34::decrypt_message(alice_to_bob, bob_secret);
        const auto bob_to_alice = ex34::encrypt_message(echoed_message, bob_secret, bob_iv);

        return {
            malicious_generator,
            alice_public_key,
            bob_public_key,
            predictable_secret,
            alice_to_bob,
            bob_to_alice,
            ex34::decrypt_message(alice_to_bob, predictable_secret),
            ex34::decrypt_message(bob_to_alice, predictable_secret)};
    }

} // namespace set05::ex35
