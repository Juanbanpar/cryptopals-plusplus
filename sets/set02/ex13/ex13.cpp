#include "ex13.h"
#include "common.h"
#include "aes.h"
#include "pkcs7.h"
#include <sstream>
#include <algorithm>

namespace set02::ex13
{

    namespace
    {
        std::vector<unsigned char> KEY;
        bool initialized = false;

        const std::vector<unsigned char> &get_key()
        {
            if (!initialized)
            {
                KEY = rand_bytes(16);
                initialized = true;
            }
            return KEY;
        }
    }

    std::map<std::string, std::string> parse_kv(const std::string &kv_string)
    {
        std::map<std::string, std::string> res;
        std::stringstream ss(kv_string);
        std::string segment;
        while (std::getline(ss, segment, '&'))
        {
            size_t eq = segment.find('=');
            if (eq != std::string::npos)
            {
                res[segment.substr(0, eq)] = segment.substr(eq + 1);
            }
        }
        return res;
    }

    std::string profile_to_string(const UserProfile &profile)
    {
        return "email=" + profile.email + "&uid=" + std::to_string(profile.uid) + "&role=" + profile.role;
    }

    std::vector<unsigned char> encrypt_profile_for(std::string email)
    {
        // Sanitize
        email.erase(std::remove(email.begin(), email.end(), '&'), email.end());
        email.erase(std::remove(email.begin(), email.end(), '='), email.end());

        UserProfile p;
        p.email = email;
        p.uid = 10;
        p.role = "user";

        std::string encoded = profile_to_string(p);
        std::vector<unsigned char> input(encoded.begin(), encoded.end());
        return aes::encrypt_ecb(input, get_key());
    }

    UserProfile decrypt_profile(const std::vector<unsigned char> &ciphertext)
    {
        std::vector<unsigned char> plaintext = aes::decrypt_ecb(ciphertext, get_key());
        // Unpad done by decrypt_ecb
        std::string s(plaintext.begin(), plaintext.end());
        auto kv = parse_kv(s);

        UserProfile p;
        p.email = kv["email"];
        p.uid = std::stoi(kv["uid"]);
        p.role = kv["role"];
        return p;
    }

    std::vector<unsigned char> create_admin_profile()
    {
        // Build a single AES block that contains "admin" plus PKCS#7 padding.
        std::string admin_payload = "admin";
        std::vector<unsigned char> admin_bytes(admin_payload.begin(), admin_payload.end());
        admin_bytes = pkcs7::pad(admin_bytes, 16);
        std::string admin_str(admin_bytes.begin(), admin_bytes.end());

        // Craft an email so that the padded "admin" becomes its own ciphertext block,
        // then extract that ciphertext block.
        std::string email1 = std::string(10, 'A') + admin_str;
        auto cipher1 = encrypt_profile_for(email1);
        std::vector<unsigned char> admin_block(cipher1.begin() + 16, cipher1.begin() + 32);

        // Create a normal profile where "role=" is aligned to a block boundary,
        // so the following block holds the role text we can replace.
        std::string email2 = std::string(13, 'B');
        auto cipher2 = encrypt_profile_for(email2);

        // Replace the role block in cipher2 with our prepared admin block.
        std::vector<unsigned char> final_cipher(cipher2.begin(), cipher2.begin() + 32);
        final_cipher.insert(final_cipher.end(), admin_block.begin(), admin_block.end());

        return final_cipher;
    }

} // namespace set02::ex13
