#include "ex17.h"
#include "common.h"
#include "aes.h"
#include "pkcs7.h"
#include <random>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace set03::ex17
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

        const std::vector<std::string> STRINGS = {
            "MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=",
            "MDAwMDAxV2l0aCB0aGUgYmFzcyBraWNrZWQgaW4gYW5kIHRoZSBWZWdhJ3MgYXJlIHB1bXBpbic=",
            "MDAwMDAyUXVpY2sgdG8gdGhlIHBvaW50LCB0byB0aGUgcG9pbnQsIG5vIGZha2luZw==",
            "MDAwMDAzQ29va2luZyBNQydzIGxpa2UgYSBwb3VuZCBvZiBiYWNvbg==",
            "MDAwMDA0QnVybmluZyAnZW0sIGlmIHlvdSBhaW4ndCBxdWljayBhbmQgbmltYmxl",
            "MDAwMDA1SSBnbyBjcmF6eSB3aGVuIEkgaGVhciBhIGN5bWJhbA==",
            "MDAwMDA2QW5kIGEgaGlnaCBoYXQgd2l0aCBhIHNvdXBlZCB1cCB0ZW1wbw==",
            "MDAwMDA3SSdtIG9uIGEgcm9sbCwgaXQncyB0aW1lIHRvIGdvIHNvbG8=",
            "MDAwMDA4b2xsaW4nIGluIG15IGZpdmUgcG9pbnQgb2g=",
            "MDAwMDA5aXRoIG15IHJhZy10b3AgZG93biBzbyBteSBoYWlyIGNhbiBibG93"};
    }

    std::pair<std::vector<unsigned char>, std::vector<unsigned char>> encrypt_random_string()
    {
        ensure_init();

        // Pick a random string
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, STRINGS.size() - 1);
        auto plaintext = base64_to_bytes(STRINGS[dist(gen)]);

        // Generate random IV and encrypt
        auto iv = rand_bytes(16);
        auto ciphertext = aes::encrypt_cbc(plaintext, KEY, iv);

        return {ciphertext, iv};
    }

    bool check_padding(const std::vector<unsigned char> &ciphertext, const std::vector<unsigned char> &iv)
    {
        ensure_init();
        try
        {
            aes::decrypt_cbc(ciphertext, KEY, iv, true); // unpad=true throws on bad padding
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    std::vector<unsigned char> padding_oracle_attack(
        const std::vector<unsigned char> &ciphertext,
        const std::vector<unsigned char> &iv)
    {
        if (ciphertext.size() % 16 != 0)
            throw std::invalid_argument("Ciphertext must be multiple of 16 bytes");

        std::vector<unsigned char> recovered;
        size_t num_blocks = ciphertext.size() / 16;

        // Process each block from the end
        for (size_t block = 0; block < num_blocks; ++block)
        {
            // Current block to decrypt
            std::vector<unsigned char> curr_block(
                ciphertext.begin() + block * 16,
                ciphertext.begin() + (block + 1) * 16);

            std::vector<unsigned char> intermediate(16, 0);
            std::vector<unsigned char> decrypted(16, 0);

            // Attack each byte from the end
            for (int byte_idx = 15; byte_idx >= 0; --byte_idx)
            {
                unsigned char pad_val = 16 - byte_idx;

                // Prepare modified IV (for first block) or previous block (for others)
                std::vector<unsigned char> modified_iv;
                if (block == 0)
                    modified_iv = iv;
                else
                    modified_iv = std::vector<unsigned char>(
                        ciphertext.begin() + (block - 1) * 16,
                        ciphertext.begin() + block * 16);

                // Set up bytes we've already discovered
                for (int j = byte_idx + 1; j < 16; ++j)
                {
                    modified_iv[j] = intermediate[j] ^ pad_val;
                }

                // Try all possible byte values
                bool found = false;
                for (int guess = 0; guess < 256; ++guess)
                {
                    modified_iv[byte_idx] = (unsigned char)guess;

                    if (check_padding(curr_block, modified_iv))
                    {
                        // Verify it's the right padding byte (not a false positive)
                        if (byte_idx > 0)
                        {
                            std::vector<unsigned char> verify = modified_iv;
                            verify[byte_idx - 1] ^= 0x01;
                            if (!check_padding(curr_block, verify))
                                continue;
                        }

                        intermediate[byte_idx] = guess ^ pad_val;
                        // XOR with original prev block/IV to get plaintext
                        if (block == 0)
                            decrypted[byte_idx] = intermediate[byte_idx] ^ iv[byte_idx];
                        else
                            decrypted[byte_idx] = intermediate[byte_idx] ^ ciphertext[(block - 1) * 16 + byte_idx];
                        found = true;
                        break;
                    }
                }

                if (!found)
                    throw std::runtime_error("Failed to recover byte");
            }

            recovered.insert(recovered.end(), decrypted.begin(), decrypted.end());
        }

        // Strip PKCS#7 padding
        try
        {
            return pkcs7::unpad(recovered);
        }
        catch (...)
        {
            return recovered;
        }
    }

} // namespace set03::ex17
