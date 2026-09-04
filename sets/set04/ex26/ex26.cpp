#include "ex26.h"
#include "aes.h"
#include "common.h"

namespace set04::ex26
{

    namespace
    {
        std::vector<unsigned char> KEY;
        bool initialized = false;

        void ensure_init()
        {
            if (!initialized)
            {
                KEY = rand_bytes(16);
                initialized = true;
            }
        }

        std::string sanitize(const std::string &userdata)
        {
            std::string sanitized;
            for (unsigned char c : userdata)
            {
                if (c == ';' || c == '=')
                {
                    static const char digits[] = "0123456789ABCDEF";
                    sanitized += '%';
                    sanitized += digits[c >> 4];
                    sanitized += digits[c & 0x0f];
                }
                else
                {
                    sanitized += static_cast<char>(c);
                }
            }
            return sanitized;
        }
    }

    std::vector<unsigned char> encrypt_userdata(const std::string &userdata)
    {
        ensure_init();
        const std::string prefix = "comment1=cooking%20MCs;userdata=";
        const std::string suffix = ";comment2=%20like%20a%20pound%20of%20bacon";
        const std::string plaintext = prefix + sanitize(userdata) + suffix;
        return aes::ctr(
            std::vector<unsigned char>(plaintext.begin(), plaintext.end()),
            KEY,
            0
        );
    }

    bool is_admin(const std::vector<unsigned char> &ciphertext)
    {
        ensure_init();
        const auto plaintext = aes::ctr(ciphertext, KEY, 0);
        const std::string value(plaintext.begin(), plaintext.end());
        return value.find(";admin=true;") != std::string::npos;
    }

    std::vector<unsigned char> create_admin_ciphertext()
    {
        const std::string payload = "AadminAtrueA";
        auto ciphertext = encrypt_userdata(payload);

        const size_t payload_offset = std::string("comment1=cooking%20MCs;userdata=").size();
        ciphertext[payload_offset] ^= static_cast<unsigned char>('A' ^ ';');
        ciphertext[payload_offset + 6] ^= static_cast<unsigned char>('A' ^ '=');
        ciphertext[payload_offset + 11] ^= static_cast<unsigned char>('A' ^ ';');
        return ciphertext;
    }

} // namespace set04::ex26
