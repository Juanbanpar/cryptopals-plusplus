#pragma once

#include <cstddef>
#include <vector>

namespace set04::ex25
{

    // Encrypt the recovered Set 1 challenge plaintext under a random CTR key.
    std::vector<unsigned char> encrypt_challenge_plaintext();

    // Edit an AES-CTR ciphertext using the supplied key and nonce zero.
    std::vector<unsigned char> edit_ciphertext(
        const std::vector<unsigned char> &ciphertext,
        const std::vector<unsigned char> &key,
        size_t offset,
        const std::vector<unsigned char> &newtext
    );

    // Simulate the vulnerable edit API without exposing the random key.
    std::vector<unsigned char> edit_oracle(
        const std::vector<unsigned char> &ciphertext,
        size_t offset,
        const std::vector<unsigned char> &newtext
    );

    // Recover the original plaintext through the edit oracle.
    std::vector<unsigned char> recover_plaintext(const std::vector<unsigned char> &ciphertext);

} // namespace set04::ex25
