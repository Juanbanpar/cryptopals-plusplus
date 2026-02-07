#pragma once

#include <string>

namespace set01::ex05
{

    // Repeating-key XOR: returns hex string of ciphertext
    std::string repeating_key_xor_hex_from_text(const std::string &text, const std::string &key);

} // namespace set01::ex05
