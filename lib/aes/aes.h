#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace aes
{

    // Core block operations
    void encrypt_block(const uint8_t in[16], uint8_t out[16], const uint8_t *round_keys);
    void decrypt_block(const uint8_t in[16], uint8_t out[16], const uint8_t *round_keys);

    // Key expansion for AES-128
    std::vector<uint8_t> key_expansion(const std::vector<uint8_t> &key);

    // ECB Mode
    std::vector<uint8_t> encrypt_ecb(const std::vector<uint8_t> &plaintext, const std::vector<uint8_t> &key, bool pad = true);
    std::vector<uint8_t> decrypt_ecb(const std::vector<uint8_t> &ciphertext, const std::vector<uint8_t> &key, bool unpad = true);

    // CBC Mode
    std::vector<uint8_t> encrypt_cbc(const std::vector<uint8_t> &plaintext, const std::vector<uint8_t> &key, const std::vector<uint8_t> &iv, bool pad = true);
    std::vector<uint8_t> decrypt_cbc(const std::vector<uint8_t> &ciphertext, const std::vector<uint8_t> &key, const std::vector<uint8_t> &iv, bool unpad = true);

}
