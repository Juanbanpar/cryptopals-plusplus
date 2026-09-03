#pragma once
#include <vector>
#include <cstdint>

namespace set03::ex24
{

// MT19937 stream cipher: encrypt/decrypt using 16-bit seed
std::vector<unsigned char> mt19937_cipher(const std::vector<unsigned char> &data, uint16_t seed);

// Encrypt known plaintext with random prefix
std::vector<unsigned char> encrypt_with_random_prefix(const std::vector<unsigned char> &known_plaintext, uint16_t seed);

// Recover 16-bit seed from ciphertext with known plaintext suffix
uint16_t recover_seed(const std::vector<unsigned char> &ciphertext, const std::vector<unsigned char> &known_plaintext);

// Generate password reset token using MT19937 seeded with current time
std::vector<unsigned char> generate_password_token();

// Check if token was generated using MT19937 seeded with recent timestamp
bool is_mt19937_time_token(const std::vector<unsigned char> &token, uint32_t max_seconds_back = 60);

} // namespace set03::ex24
