#include "ex27.h"
#include "aes.h"
#include "common.h"
#include "pkcs7.h"
#include <algorithm>

namespace set04::ex27
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

        std::vector<unsigned char> first_three_blocks(const std::vector<unsigned char> &ciphertext)
        {
            if (ciphertext.size() < 48 || ciphertext.size() % 16 != 0)
                throw std::invalid_argument("CBC key recovery requires at least three blocks");

            std::vector<unsigned char> modified(48, 0);
            std::copy(ciphertext.begin(), ciphertext.begin() + 16, modified.begin());
            std::copy(ciphertext.begin(), ciphertext.begin() + 16, modified.begin() + 32);
            return modified;
        }
    }

    InvalidAsciiError::InvalidAsciiError(const std::vector<unsigned char> &plaintext)
        : std::runtime_error("Invalid ASCII plaintext: " + bytes_to_hex(plaintext)), plaintext_(plaintext)
    {
    }

    const std::vector<unsigned char> &InvalidAsciiError::plaintext() const noexcept
    {
        return plaintext_;
    }

    std::vector<unsigned char> encrypt_message(const std::vector<unsigned char> &plaintext)
    {
        ensure_init();
        return aes::encrypt_cbc(plaintext, KEY, KEY);
    }

    std::vector<unsigned char> decrypt_message(const std::vector<unsigned char> &ciphertext)
    {
        ensure_init();
        const auto plaintext = aes::decrypt_cbc(ciphertext, KEY, KEY, false);
        for (unsigned char c : plaintext)
        {
            if (c > 0x7f)
                throw InvalidAsciiError(plaintext);
        }
        return pkcs7::unpad(plaintext);
    }

    std::vector<unsigned char> recover_key(const std::vector<unsigned char> &ciphertext)
    {
        const auto modified = first_three_blocks(ciphertext);

        try
        {
            decrypt_message(modified);
        }
        catch (const InvalidAsciiError &error)
        {
            const auto &plaintext = error.plaintext();
            std::vector<unsigned char> key(16);
            for (size_t i = 0; i < key.size(); ++i)
                key[i] = plaintext[i] ^ plaintext[32 + i];
            return key;
        }

        throw std::runtime_error("Modified CBC ciphertext did not produce an ASCII error");
    }

} // namespace set04::ex27
