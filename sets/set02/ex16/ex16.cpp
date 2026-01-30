#include "ex16.h"
#include "common.h"
#include "aes.h"
#include "pkcs7.h"
#include <iostream>
#include <algorithm>

namespace set02::ex16
{

    namespace
    {
        std::vector<unsigned char> KEY;
        std::vector<unsigned char> IV;
        bool initialized = false;

        void ensure_init()
        {
            if (!initialized)
            {
                KEY = rand_bytes(16);
                IV = rand_bytes(16);
                initialized = true;
            }
        }
    }

    std::vector<unsigned char> encrypt_userdata(const std::string &userdata)
    {
        ensure_init();
        // Sanitize input by percent-encoding ';' and '=' so they cannot be injected
        std::string sanitized;
        for (char c : userdata)
        {
            if (c == ';' || c == '=')
            {
                sanitized += "%";
                char buf[3];
                sprintf(buf, "%02X", (unsigned char)c);
                sanitized += buf;
            }
            else
            {
                sanitized += c;
            }
        }

        const std::string prefix = "comment1=cooking%20MCs;userdata=";
        const std::string suffix = ";comment2=%20like%20a%20pound%20of%20bacon";

        std::string full = prefix + sanitized + suffix;
        std::vector<unsigned char> plain(full.begin(), full.end());

        return aes::encrypt_cbc(plain, KEY, IV);
    }

    bool is_admin(const std::vector<unsigned char> &ciphertext)
    {
        ensure_init();
        // decrypt_cbc unpads by default
        auto plain = aes::decrypt_cbc(ciphertext, KEY, IV);

        std::string s(plain.begin(), plain.end());
        // Search for ";admin=true;"
        return s.find(";admin=true;") != std::string::npos;
    }

    std::vector<unsigned char> create_admin_ciphertext()
    {
        // Create userdata so the bytes we control fall exactly in a block we can modify
        // Use 'A' placeholders where we will flip bits to get ";admin=true;"
        std::string payload = "AadminAtrueA"; // 'A' will be flipped to ';' and '='

        auto ciphertext = encrypt_userdata(payload);

        // Payload begins at byte offset 32 (two 16-byte prefix blocks).
        // To change plaintext bytes at offsets 32, 38, 43, flip corresponding bytes in previous block (offsets 16, 22, 27).
        ciphertext[16] ^= (0x41 ^ 0x3B);      // 'A' -> ';'
        ciphertext[16 + 6] ^= (0x41 ^ 0x3D);  // 'A' -> '='
        ciphertext[16 + 11] ^= (0x41 ^ 0x3B); // 'A' -> ';'

        return ciphertext;
    }

} // namespace set02::ex16
