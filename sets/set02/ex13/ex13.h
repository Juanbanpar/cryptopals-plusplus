#pragma once

#include <string>
#include <map>
#include <vector>

namespace set02::ex13
{

    struct UserProfile
    {
        std::string email;
        int uid;
        std::string role;
    };

    // Parses "foo=bar&baz=qux"
    std::map<std::string, std::string> parse_kv(const std::string &kv_string);

    // Encodes profile to string
    std::string profile_to_string(const UserProfile &profile);

    // Generates a profile for an email (sanitizing inputs), encodes it, encrypts with fixed key.
    std::vector<unsigned char> encrypt_profile_for(std::string email);

    // Decrypts and parses the profile.
    UserProfile decrypt_profile(const std::vector<unsigned char> &ciphertext);

    // Attack function: generates a ciphertext that parses to a profile with role=admin
    std::vector<unsigned char> create_admin_profile();

} // namespace set02::ex13