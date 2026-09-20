#include "ex40.h"

#include <stdexcept>

namespace set05::ex40
{

    std::array<BroadcastCiphertext, 3> encrypt_for_recipients(
        const std::vector<unsigned char> &message,
        const std::array<ex39::KeyPair, 3> &recipients
    )
    {
        std::array<BroadcastCiphertext, 3> result{};
        for (size_t i = 0; i < result.size(); ++i)
        {
            result[i] = {
                recipients[i].public_key,
                ex39::encrypt_message(message, recipients[i].public_key)};
        }
        return result;
    }

    bignum::BigUint recover_message(
        const std::array<BroadcastCiphertext, 3> &ciphertexts
    )
    {
        const auto &n0 = ciphertexts[0].public_key.n;
        const auto &n1 = ciphertexts[1].public_key.n;
        const auto &n2 = ciphertexts[2].public_key.n;
        const bignum::BigUint modulus_product = n0 * n1 * n2;
        bignum::BigUint accumulated(0);

        for (const auto &ciphertext : ciphertexts)
        {
            const auto &modulus = ciphertext.public_key.n;
            const auto partial_modulus = modulus_product / modulus;
            const auto inverse = bignum::mod_inverse(
                partial_modulus % modulus,
                modulus
            );
            accumulated += ciphertext.ciphertext * partial_modulus * inverse;
        }

        const auto combined = accumulated % modulus_product;
        const auto message = bignum::integer_cube_root(combined);
        if (message * message * message != combined)
            throw std::invalid_argument("broadcast ciphertext does not have an exact cube root");
        return message;
    }

    std::vector<unsigned char> recover_message_bytes(
        const std::array<BroadcastCiphertext, 3> &ciphertexts,
        size_t width
    )
    {
        return ex39::integer_to_bytes(recover_message(ciphertexts), width);
    }

} // namespace set05::ex40
