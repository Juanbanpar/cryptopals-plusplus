#pragma once

#include "ex36.h"
#include <cstddef>
#include <cstdint>

namespace set05::ex38
{

    struct ClientHello
    {
        std::string identity;
        bignum::BigUint public_key;
    };

    struct Challenge
    {
        std::vector<unsigned char> salt;
        bignum::BigUint public_key;
        bignum::BigUint scrambling_parameter;
    };

    struct AttackerChallenge
    {
        Challenge challenge;
        bignum::BigUint private_key;
    };

    class Client
    {
    public:
        Client(
            const ex36::Parameters &parameters,
            const std::string &identity,
            const std::vector<unsigned char> &password,
            uint64_t private_key = 0
        );

        ClientHello hello() const;
        std::vector<unsigned char> response(const Challenge &challenge) const;

    private:
        ex36::Parameters parameters_;
        std::string identity_;
        std::vector<unsigned char> password_;
        bignum::BigUint private_key_;
    };

    AttackerChallenge make_attacker_challenge(
        const ex36::Parameters &parameters,
        const std::vector<unsigned char> &salt,
        const bignum::BigUint &private_key,
        const bignum::BigUint &scrambling_parameter
    );

    struct DictionaryResult
    {
        std::vector<unsigned char> password;
        size_t attempts;
        bool found;
    };

    DictionaryResult offline_dictionary_attack(
        const ex36::Parameters &parameters,
        const ClientHello &hello,
        const AttackerChallenge &attacker_challenge,
        const std::vector<unsigned char> &captured_hmac,
        const std::vector<std::vector<unsigned char>> &dictionary
    );

} // namespace set05::ex38
