#pragma once

#include <vector>
#include <string>

namespace set03::ex19
{

    // Encrypt all plaintexts with fixed nonce (returns ciphertexts)
    std::vector<std::vector<unsigned char>> encrypt_plaintexts();

    // Break fixed-nonce CTR using manual substitution attack
    std::vector<std::string> break_fixed_nonce_ctr(const std::vector<std::vector<unsigned char>> &ciphertexts);

} // namespace set03::ex19
