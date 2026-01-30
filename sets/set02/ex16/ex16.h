#pragma once

#include <vector>
#include <string>

namespace set02::ex16
{

    // Prepares string, encrypts AES-CBC with random key/IV
    std::vector<unsigned char> encrypt_userdata(const std::string &userdata);

    // Decrypts and checks for ";admin=true;"
    bool is_admin(const std::vector<unsigned char> &ciphertext);

    // Attack: modify ciphertext to inject admin=true
    std::vector<unsigned char> create_admin_ciphertext();

} // namespace set02::ex16
