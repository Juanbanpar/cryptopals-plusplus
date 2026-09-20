#include "ex36.h"
#include "common.h"
#include "ex33.h"
#include "sha256.h"

#include <random>
#include <stdexcept>

namespace set05::ex36
{

    namespace
    {
        constexpr size_t SHA256_BLOCK_SIZE = 64;
        constexpr unsigned char INNER_PAD = 0x36;
        constexpr unsigned char OUTER_PAD = 0x5c;

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

        std::vector<unsigned char> integer_input(const BigUint &value)
        {
            return value.to_bytes_be();
        }

        BigUint hash_to_integer(const std::vector<unsigned char> &data)
        {
            return BigUint::from_bytes_be(sha256::digest(data));
        }

        BigUint salt_password_hash(
            const std::vector<unsigned char> &salt,
            const std::vector<unsigned char> &password
        )
        {
            return hash_to_integer(concatenate(salt, password));
        }

        BigUint choose_private_key(const BigUint &modulus, uint64_t private_key)
        {
            if (private_key != 0)
            {
                if (BigUint(private_key) >= modulus)
                    throw std::invalid_argument("SRP private key is out of range");
                return BigUint(private_key);
            }
            return bignum::random_below(modulus - BigUint(2), generator()) + BigUint(2);
        }

        BigUint reduced_difference(
            const BigUint &left,
            const BigUint &right,
            const BigUint &modulus
        )
        {
            const BigUint reduced_left = left % modulus;
            const BigUint reduced_right = right % modulus;
            if (reduced_left >= reduced_right)
                return reduced_left - reduced_right;
            return modulus - (reduced_right - reduced_left);
        }

        std::vector<unsigned char> hmac_key_block(
            const std::vector<unsigned char> &key
        )
        {
            std::vector<unsigned char> block = key;
            if (block.size() > SHA256_BLOCK_SIZE)
                block = sha256::digest(block);
            block.resize(SHA256_BLOCK_SIZE, 0);
            return block;
        }
    }

    Parameters toy_parameters()
    {
        return {BigUint(37), BigUint(5), BigUint(3)};
    }

    Parameters default_parameters()
    {
        const auto dh_parameters = ex33::default_parameters();
        return {dh_parameters.p, BigUint(2), BigUint(3)};
    }

    std::vector<unsigned char> hmac_sha256(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message
    )
    {
        const auto key_block = hmac_key_block(key);
        std::vector<unsigned char> inner_input;
        std::vector<unsigned char> outer_input;
        inner_input.reserve(SHA256_BLOCK_SIZE + message.size());
        outer_input.reserve(SHA256_BLOCK_SIZE + 32);

        for (unsigned char byte : key_block)
            inner_input.push_back(byte ^ INNER_PAD);
        inner_input.insert(inner_input.end(), message.begin(), message.end());

        for (unsigned char byte : key_block)
            outer_input.push_back(byte ^ OUTER_PAD);
        const auto inner_digest = sha256::digest(inner_input);
        outer_input.insert(outer_input.end(), inner_digest.begin(), inner_digest.end());
        return sha256::digest(outer_input);
    }

    std::vector<unsigned char> derive_session_key(const BigUint &shared_secret)
    {
        return sha256::digest(integer_input(shared_secret));
    }

    Server::Server(
        const Parameters &parameters,
        const std::string &identity,
        const std::vector<unsigned char> &password,
        const std::vector<unsigned char> &salt,
        uint64_t private_key
    )
        : parameters_(parameters),
          identity_(identity),
          salt_(salt.empty() ? rand_bytes(16) : salt),
          verifier_(0),
          private_key_(choose_private_key(parameters.n, private_key)),
          last_client_public_key_(0),
          last_challenge_(),
          has_challenge_(false)
    {
        if (parameters_.n <= BigUint(2) || parameters_.g.is_zero() || parameters_.g >= parameters_.n)
            throw std::invalid_argument("invalid SRP parameters");
        const BigUint x = salt_password_hash(salt_, password);
        verifier_ = bignum::mod_pow(parameters_.g, x, parameters_.n);
    }

    ServerChallenge Server::begin(const BigUint &client_public_key)
    {
        last_client_public_key_ = client_public_key;
        const BigUint gb = bignum::mod_pow(
            parameters_.g, private_key_, parameters_.n
        );
        const BigUint public_key =
            (parameters_.k * verifier_ + gb) % parameters_.n;
        const auto public_key_data = concatenate(
            integer_input(client_public_key),
            integer_input(public_key)
        );
        last_challenge_ = {
            salt_,
            public_key,
            hash_to_integer(public_key_data)};
        has_challenge_ = true;
        return last_challenge_;
    }

    bool Server::verify(
        const BigUint &client_public_key,
        const std::vector<unsigned char> &client_hmac
    ) const
    {
        if (!has_challenge_ || client_public_key != last_client_public_key_)
            return false;

        const BigUint avu = (
            client_public_key *
            bignum::mod_pow(verifier_, last_challenge_.scrambling_parameter, parameters_.n)
        ) % parameters_.n;
        const BigUint shared_secret = bignum::mod_pow(
            avu, private_key_, parameters_.n
        );
        return hmac_sha256(
                   derive_session_key(shared_secret),
                   salt_
               ) == client_hmac;
    }

    const Parameters &Server::parameters() const
    {
        return parameters_;
    }

    const std::string &Server::identity() const
    {
        return identity_;
    }

    const std::vector<unsigned char> &Server::salt() const
    {
        return salt_;
    }

    const BigUint &Server::verifier() const
    {
        return verifier_;
    }

    Client::Client(
        const Parameters &parameters,
        const std::string &identity,
        const std::vector<unsigned char> &password,
        uint64_t private_key
    )
        : parameters_(parameters),
          identity_(identity),
          password_(password),
          private_key_(choose_private_key(parameters.n, private_key))
    {
        if (parameters_.n <= BigUint(2) || parameters_.g.is_zero() || parameters_.g >= parameters_.n)
            throw std::invalid_argument("invalid SRP parameters");
    }

    ClientHello Client::hello() const
    {
        return {
            identity_,
            bignum::mod_pow(parameters_.g, private_key_, parameters_.n)};
    }

    std::vector<unsigned char> Client::response(
        const ServerChallenge &challenge
    ) const
    {
        const BigUint x = salt_password_hash(challenge.salt, password_);
        const BigUint gx = bignum::mod_pow(parameters_.g, x, parameters_.n);
        const BigUint base = reduced_difference(
            challenge.public_key,
            parameters_.k * gx,
            parameters_.n
        );
        const BigUint u = challenge.scrambling_parameter;
        const BigUint exponent = private_key_ + u * x;
        const BigUint shared_secret = bignum::mod_pow(base, exponent, parameters_.n);
        return hmac_sha256(derive_session_key(shared_secret), challenge.salt);
    }

    bool authenticate(Server &server, const Client &client)
    {
        const auto hello = client.hello();
        if (hello.identity != server.identity())
            return false;
        const auto challenge = server.begin(hello.public_key);
        return server.verify(hello.public_key, client.response(challenge));
    }

} // namespace set05::ex36
