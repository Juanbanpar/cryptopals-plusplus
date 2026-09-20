#pragma once

#include "bignum.h"
#include <cstdint>
#include <string>
#include <vector>

namespace set05::ex36
{

    using BigUint = bignum::BigUint;

    struct Parameters
    {
        BigUint n;
        BigUint g;
        BigUint k;
    };

    Parameters toy_parameters();
    Parameters default_parameters();

    std::vector<unsigned char> hmac_sha256(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message
    );

    std::vector<unsigned char> derive_session_key(const BigUint &shared_secret);

    struct ClientHello
    {
        std::string identity;
        BigUint public_key;
    };

    struct ServerChallenge
    {
        std::vector<unsigned char> salt;
        BigUint public_key;
        BigUint scrambling_parameter;
    };

    class Server
    {
    public:
        Server(
            const Parameters &parameters,
            const std::string &identity,
            const std::vector<unsigned char> &password,
            const std::vector<unsigned char> &salt = {},
            uint64_t private_key = 0
        );

        ServerChallenge begin(const BigUint &client_public_key);

        bool verify(
            const BigUint &client_public_key,
            const std::vector<unsigned char> &client_hmac
        ) const;

        const Parameters &parameters() const;
        const std::string &identity() const;
        const std::vector<unsigned char> &salt() const;
        const BigUint &verifier() const;

    private:
        Parameters parameters_;
        std::string identity_;
        std::vector<unsigned char> salt_;
        BigUint verifier_;
        BigUint private_key_;
        BigUint last_client_public_key_;
        ServerChallenge last_challenge_;
        bool has_challenge_;
    };

    class Client
    {
    public:
        Client(
            const Parameters &parameters,
            const std::string &identity,
            const std::vector<unsigned char> &password,
            uint64_t private_key = 0
        );

        ClientHello hello() const;

        std::vector<unsigned char> response(const ServerChallenge &challenge) const;

    private:
        Parameters parameters_;
        std::string identity_;
        std::vector<unsigned char> password_;
        BigUint private_key_;
    };

    bool authenticate(Server &server, const Client &client);

} // namespace set05::ex36
