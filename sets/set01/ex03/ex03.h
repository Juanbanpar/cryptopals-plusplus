#pragma once

#include "common.h"

#include <string>
#include <utility>

namespace set01::ex03
{

    // Crack single-byte XOR. Returns pair<key, plaintext>
    std::pair<unsigned char, std::string> crack_single_byte_xor(const std::string &hex);

} // namespace set01::ex03
