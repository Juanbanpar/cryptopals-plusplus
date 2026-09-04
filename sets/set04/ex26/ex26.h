#pragma once

#include <string>
#include <vector>

namespace set04::ex26
{

    // Encrypt sanitized userdata with AES-CTR using a fixed nonce.
    std::vector<unsigned char> encrypt_userdata(const std::string &userdata);

    // Decrypt and check for the admin token.
    bool is_admin(const std::vector<unsigned char> &ciphertext);

    // Modify CTR ciphertext bytes to inject an admin token.
    std::vector<unsigned char> create_admin_ciphertext();

} // namespace set04::ex26
