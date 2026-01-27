#pragma once

#include <string>

namespace set01 { namespace ex08 {
// returns the hex line in file that is most likely AES-128-ECB (repeated 16-byte blocks)
std::string detect_aes_128_ecb_in_hex_file(const std::string &path);

} }
