#pragma once

#include <vector>
#include <string>

namespace set03::ex17
{

    // Encrypts a random string from the challenge set, returns ciphertext and IV
    std::pair<std::vector<unsigned char>, std::vector<unsigned char>> encrypt_random_string();

    // Decrypts and checks padding validity (the oracle)
    bool check_padding(const std::vector<unsigned char> &ciphertext, const std::vector<unsigned char> &iv);

    // Performs the padding oracle attack to recover the plaintext
    std::vector<unsigned char> padding_oracle_attack(
        const std::vector<unsigned char> &ciphertext,
        const std::vector<unsigned char> &iv
    );

} // namespace set03::ex17
