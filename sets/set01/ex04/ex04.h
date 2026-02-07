#pragma once

#include "common.h"

#include <string>
#include <tuple>

namespace set01::ex04
{

    // Scans file at `path` (one hex string per line) and returns
    // tuple<best_plaintext, key, hex_line>
    std::tuple<std::string, unsigned char, std::string> detect_single_char_xor(const std::string &path);

} // namespace set01::ex04
