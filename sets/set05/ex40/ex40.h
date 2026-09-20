#pragma once

#include "ex39.h"
#include <array>

namespace set05::ex40
{

    struct BroadcastCiphertext
    {
        ex39::PublicKey public_key;
        bignum::BigUint ciphertext;
    };

    std::array<BroadcastCiphertext, 3> encrypt_for_recipients(
        const std::vector<unsigned char> &message,
        const std::array<ex39::KeyPair, 3> &recipients
    );

    bignum::BigUint recover_message(
        const std::array<BroadcastCiphertext, 3> &ciphertexts
    );

    std::vector<unsigned char> recover_message_bytes(
        const std::array<BroadcastCiphertext, 3> &ciphertexts,
        size_t width = 0
    );

} // namespace set05::ex40
