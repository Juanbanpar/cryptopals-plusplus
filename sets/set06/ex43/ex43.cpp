#include "ex43.h"

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
}

namespace set06::ex43
{

    Recovery recover_key_from_small_nonce(
        const std::vector<unsigned char> &message,
        const dsa::Signature &signature,
        const dsa::PublicKey &public_key,
        uint64_t nonce_limit
    )
    {
        const auto &q = public_key.parameters.q;
        const auto hash = dsa::message_hash(message);
        const auto inverse_r = bignum::mod_inverse(signature.r, q);
        for (uint64_t candidate = 1; candidate < nonce_limit; ++candidate)
        {
            const bignum::BigUint nonce(candidate);
            if (bignum::mod_pow(
                    public_key.parameters.g,
                    nonce,
                    public_key.parameters.p
                ) % q != signature.r)
                continue;
            const auto numerator = dsa::subtract_mod(
                signature.s * nonce,
                hash,
                q
            );
            const auto private_key = (numerator * inverse_r) % q;
            if (bignum::mod_pow(
                    public_key.parameters.g,
                    private_key,
                    public_key.parameters.p
                ) == public_key.y)
                return {private_key, nonce, true};
        }
        return {bignum::BigUint(0), bignum::BigUint(0), false};
    }

    dsa::PublicKey challenge_public_key()
    {
        auto parameters = dsa::challenge_parameters();
        return {
            parameters,
            bignum::BigUint::from_hex(
                "84ad4719d044495496a3201c8ff484feb45b962e7302e56a392aee4abab3e4"
                "bdebf2955b4736012f21a08084056b19bcd7fee56048e004e44984e2f411788e"
                "fdc837a0d2e5abb7b555039fd243ac01f0fb2ed1dec568280ce678e931868d23"
                "eb095fde9d3779191b8c0299d6e07bbb283e6633451e535c45513b2d33c99ea17")};
    }

    dsa::Signature challenge_signature()
    {
        return {
            from_decimal("548099063082341131477253921760299949438196259240"),
            from_decimal("857042759984254168557880549501802188789837994940")};
    }

    std::vector<unsigned char> challenge_message()
    {
        const std::string message =
            "For those that envy a MC it can be hazardous to your health\n"
            "So be friendly, a matter of life and death, just like a etch-a-sketch\n";
        return {message.begin(), message.end()};
    }

} // namespace set06::ex43
