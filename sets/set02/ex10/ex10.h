#pragma once

#include <string>
#include <vector>

namespace set02::ex10
{
    // Decrypt raw ciphertext (bytes) using AES-128-CBC with provided key and IV.
    // Returns plaintext as a std::string.
    std::string decrypt_aes_128_cbc(const std::vector<unsigned char> &cipher, const std::string &key, const std::vector<unsigned char> &iv);

} // namespace set02::ex10
