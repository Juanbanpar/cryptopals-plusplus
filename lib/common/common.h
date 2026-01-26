#pragma once

#include <string>
#include <vector>

std::vector<unsigned char> hex_to_bytes(const std::string &hex);
std::string bytes_to_base64(const std::vector<unsigned char> &data);
std::string hex_to_base64(const std::string &hex);
std::string bytes_to_hex(const std::vector<unsigned char> &data);
std::string fixed_xor_hex(const std::string &ahex, const std::string &bhex);
