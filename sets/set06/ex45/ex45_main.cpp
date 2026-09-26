#include "ex45.h"

#include <iostream>

int main()
{
    const auto parameters = set06::dsa::challenge_parameters();
    const auto honest_keys = set06::dsa::generate_key_pair(parameters);

    auto zero_generator_parameters = parameters;
    zero_generator_parameters.g = bignum::BigUint(0);
    const auto zero_generator_keys = set06::dsa::key_pair_from_private(
        zero_generator_parameters, honest_keys.private_key.x
    );
    const std::string first = "Hello, world";
    const std::vector<unsigned char> first_message(first.begin(), first.end());
    const auto zero_signature = set06::dsa::sign(
        first_message, zero_generator_keys.private_key
    );
    const std::vector<unsigned char> unrelated{'a', 't', 't', 'a', 'c', 'k'};

    auto magic_parameters = parameters;
    magic_parameters.g = parameters.p + bignum::BigUint(1);
    const set06::dsa::PublicKey vulnerable_key{
        magic_parameters, honest_keys.public_key.y};
    const auto first_magic = set06::ex45::forge_universal_signature(
        vulnerable_key, bignum::BigUint(7)
    );
    const auto second_magic = set06::ex45::forge_universal_signature(
        vulnerable_key, bignum::BigUint(13)
    );
    const std::string goodbye = "Goodbye, world";
    const std::vector<unsigned char> goodbye_message(goodbye.begin(), goodbye.end());

    const bool zero_attack = set06::dsa::verify(
        unrelated, zero_signature, zero_generator_keys.public_key
    );
    const bool magic_attack = set06::dsa::verify(
        first_message, first_magic, vulnerable_key
    ) && set06::dsa::verify(goodbye_message, second_magic, vulnerable_key);
    std::cout << "g = 0 universal verification: "
              << (zero_attack ? "SUCCESS" : "FAILURE") << '\n'
              << "g = p + 1 universal signatures: "
              << (magic_attack ? "SUCCESS" : "FAILURE") << std::endl;
    return zero_attack && magic_attack ? 0 : 1;
}
