#pragma once

#include <string>
#include <utility>

namespace set01 { namespace ex06 {
// Return {key, plaintext}
std::pair<std::string, std::string> break_repeating_key_xor_from_base64_file(const std::string &path);
std::pair<std::string, std::string> break_repeating_key_xor_from_base64(const std::string &b64);

} }
