#pragma once

#include <string>
#include <vector>
#include <functional>

std::vector<unsigned char> hex_to_bytes(const std::string &hex);
std::string bytes_to_base64(const std::vector<unsigned char> &data);
std::string hex_to_base64(const std::string &hex);
std::string bytes_to_hex(const std::vector<unsigned char> &data);
std::string fixed_xor_hex(const std::string &ahex, const std::string &bhex);
double score_english(const std::string &s);
std::pair<unsigned char, std::string> crack_single_byte_xor_hex(const std::string &hex);
std::vector<unsigned char> base64_to_bytes(const std::string &b64);
int hamming_distance(const std::vector<unsigned char> &a, const std::vector<unsigned char> &b);
std::pair<unsigned char, std::string> crack_single_byte_xor_bytes(const std::vector<unsigned char> &bytes);
std::vector<unsigned char> rand_bytes(size_t n);

// Detect whether an encryption oracle is using ECB or CBC mode.
// The oracle should be a function that takes plaintext bytes and returns ciphertext bytes.
std::string detect_ecb_or_cbc(const std::function<std::vector<unsigned char>(const std::vector<unsigned char> &)> &oracle);
