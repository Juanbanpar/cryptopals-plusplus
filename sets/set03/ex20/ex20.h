#pragma once

#include <vector>
#include <string>

namespace set03::ex20
{

    // Encrypt all plaintexts from file with fixed nonce
    std::vector<std::vector<unsigned char>> encrypt_plaintexts();
    std::vector<std::vector<unsigned char>> encrypt_plaintexts(const std::string &path);

    // Break fixed-nonce CTR treating it as repeating-key XOR
    std::vector<std::string> break_fixed_nonce_ctr(const std::vector<std::vector<unsigned char>> &ciphertexts);

} // namespace set03::ex20
