#include "ex38.h"
#include "sha256.h"

#include <random>
#include <stdexcept>

namespace set05::ex38
{

    namespace
    {
        std::mt19937_64 &generator()
        {
            static std::random_device device;
            static std::mt19937_64 value(device());
            return value;
        }

        std::vector<unsigned char> concatenate(
            const std::vector<unsigned char> &left,
            const std::vector<unsigned char> &right
        )
        {
            std::vector<unsigned char> result;
            result.reserve(left.size() + right.size());
            result.insert(result.end(), left.begin(), left.end());
            result.insert(result.end(), right.begin(), right.end());
            return result;
        }

        bignum::BigUint hash_to_integer(const std::vector<unsigned char> &data)
        {
            return bignum::BigUint::from_bytes_be(sha256::digest(data));
        }

        bignum::BigUint choose_private_key(
            const bignum::BigUint &modulus,
            uint64_t private_key
        )
        {
            if (private_key != 0)
                return bignum::BigUint(private_key);
            return bignum::random_below(modulus - bignum::BigUint(2), generator()) +
                   bignum::BigUint(2);
        }

        std::vector<unsigned char> session_key(
            const ex36::Parameters &parameters,
            const bignum::BigUint &client_public_key,
            const std::vector<unsigned char> &salt,
            const std::vector<unsigned char> &password,
            const bignum::BigUint &scrambling_parameter,
            const bignum::BigUint &server_private_key
        )
        {
            const auto x = hash_to_integer(concatenate(salt, password));
            const auto verifier = bignum::mod_pow(
                parameters.g, x, parameters.n
            );
            const auto amplified_public_key = (
                client_public_key *
                bignum::mod_pow(verifier, scrambling_parameter, parameters.n)
            ) % parameters.n;
            const auto shared_secret = bignum::mod_pow(
                amplified_public_key, server_private_key, parameters.n
            );
            return ex36::derive_session_key(shared_secret);
        }
    }

    Client::Client(
        const ex36::Parameters &parameters,
        const std::string &identity,
        const std::vector<unsigned char> &password,
        uint64_t private_key
    )
        : parameters_(parameters),
          identity_(identity),
          password_(password),
          private_key_(choose_private_key(parameters.n, private_key))
    {
        if (parameters_.n <= bignum::BigUint(2) || parameters_.g.is_zero() ||
            parameters_.g >= parameters_.n)
            throw std::invalid_argument("invalid simplified SRP parameters");
    }

    ClientHello Client::hello() const
    {
        return {
            identity_,
            bignum::mod_pow(parameters_.g, private_key_, parameters_.n)};
    }

    std::vector<unsigned char> Client::response(const Challenge &challenge) const
    {
        const auto x = hash_to_integer(concatenate(challenge.salt, password_));
        const auto exponent = private_key_ + challenge.scrambling_parameter * x;
        const auto shared_secret = bignum::mod_pow(
            challenge.public_key, exponent, parameters_.n
        );
        return ex36::hmac_sha256(
            ex36::derive_session_key(shared_secret),
            challenge.salt
        );
    }

    AttackerChallenge make_attacker_challenge(
        const ex36::Parameters &parameters,
        const std::vector<unsigned char> &salt,
        const bignum::BigUint &private_key,
        const bignum::BigUint &scrambling_parameter
    )
    {
        if (private_key.is_zero())
            throw std::invalid_argument("simplified SRP private key must not be zero");
        return {
            {
                salt,
                bignum::mod_pow(parameters.g, private_key, parameters.n),
                scrambling_parameter},
            private_key};
    }

    DictionaryResult offline_dictionary_attack(
        const ex36::Parameters &parameters,
        const ClientHello &hello,
        const AttackerChallenge &attacker_challenge,
        const std::vector<unsigned char> &captured_hmac,
        const std::vector<std::vector<unsigned char>> &dictionary
    )
    {
        size_t attempts = 0;
        for (const auto &candidate : dictionary)
        {
            ++attempts;
            const auto key = session_key(
                parameters,
                hello.public_key,
                attacker_challenge.challenge.salt,
                candidate,
                attacker_challenge.challenge.scrambling_parameter,
                attacker_challenge.private_key
            );
            const auto expected = ex36::hmac_sha256(
                key,
                attacker_challenge.challenge.salt
            );
            if (expected == captured_hmac)
                return {candidate, attempts, true};
        }
        return {{}, attempts, false};
    }

} // namespace set05::ex38
