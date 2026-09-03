#pragma once

#include <string>
#include <vector>

namespace set01::ex07
{
    // decrypt base64-file encrypted with AES-128-ECB and return plaintext
    std::string decrypt_aes_128_ecb_base64_file(const std::string &path, const std::string &key = "YELLOW SUBMARINE");
    std::string decrypt_aes_128_ecb_base64(const std::vector<unsigned char> &data, const std::string &key = "YELLOW SUBMARINE");

} // namespace set01::ex07
