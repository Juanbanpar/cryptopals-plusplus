#include "ex44.h"

#include <stdexcept>

namespace
{
    bignum::BigUint from_decimal(const std::string &digits)
    {
        bignum::BigUint result(0);
        for (char digit : digits)
            result = result * bignum::BigUint(10) +
                     bignum::BigUint(static_cast<uint64_t>(digit - '0'));
        return result;
    }

    std::string value_after_colon(const std::string &line)
    {
        const auto colon = line.find(':');
        if (colon == std::string::npos)
            throw std::invalid_argument("malformed DSA signature record");
        size_t start = colon + 1;
        while (start < line.size() && line[start] == ' ')
            ++start;
        return line.substr(start);
    }
}

namespace set06::ex44
{

    std::vector<SignedMessage> load_signatures(std::istream &input)
    {
        std::vector<SignedMessage> result;
        SignedMessage current;
        bool has_message = false;
        bool has_r = false;
        bool has_s = false;
        std::string line;
        while (std::getline(input, line))
        {
            if (line.rfind("msg:", 0) == 0)
            {
                if (has_message && has_r && has_s)
                    result.push_back(current);
                current = {};
                current.message = value_after_colon(line);
                if (current.message.empty() || current.message.back() != ' ')
                    current.message.push_back(' ');
                has_message = true;
                has_r = false;
                has_s = false;
            }
            else if (line.rfind("r:", 0) == 0)
            {
                current.signature.r = from_decimal(value_after_colon(line));
                has_r = true;
            }
            else if (line.rfind("s:", 0) == 0)
            {
                current.signature.s = from_decimal(value_after_colon(line));
                has_s = true;
            }
        }
        if (has_message && has_r && has_s)
            result.push_back(current);
        return result;
    }

    dsa::PublicKey challenge_public_key()
    {
        return {
            dsa::challenge_parameters(),
            bignum::BigUint::from_hex(
                "2d026f4bf30195ede3a088da85e398ef869611d0f68f0713d51c9c1a3a26c951"
                "05d915e2d8cdf26d056b86b8a7b85519b1c23cc3ecdc6062650462e3063bd179"
                "c2a6581519f674a61f1d89a1fff27171ebc1b93d4dc57bceb7ae2430f98a6a4d"
                "83d8279ee65d71c1203d2c96d65ebbf7cce9d32971c3de5084cce04a2e147821")};
    }

    Recovery recover_reused_nonce(
        const std::vector<SignedMessage> &messages,
        const dsa::PublicKey &public_key
    )
    {
        const auto &q = public_key.parameters.q;
        for (size_t i = 0; i < messages.size(); ++i)
        {
            for (size_t j = i + 1; j < messages.size(); ++j)
            {
                const auto &first = messages[i];
                const auto &second = messages[j];
                if (first.signature.r != second.signature.r)
                    continue;
                const auto delta_s = dsa::subtract_mod(
                    first.signature.s,
                    second.signature.s,
                    q
                );
                if (delta_s.is_zero())
                    continue;
                const auto nonce = (dsa::subtract_mod(
                                        dsa::message_hash(first.message),
                                        dsa::message_hash(second.message),
                                        q
                                    ) *
                                    bignum::mod_inverse(delta_s, q)) % q;
                if (nonce.is_zero())
                    continue;
                const auto private_key = (dsa::subtract_mod(
                                              first.signature.s * nonce,
                                              dsa::message_hash(first.message),
                                              q
                                          ) *
                                          bignum::mod_inverse(first.signature.r, q)) % q;
                if (bignum::mod_pow(
                        public_key.parameters.g,
                        private_key,
                        public_key.parameters.p
                    ) == public_key.y)
                    return {private_key, nonce, i, j, true};
            }
        }
        return {
            bignum::BigUint(0),
            bignum::BigUint(0),
            0,
            0,
            false};
    }

} // namespace set06::ex44
