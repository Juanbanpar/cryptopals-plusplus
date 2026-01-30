#include "ex12.h"
#include "common.h"
#include "aes.h"
#include "pkcs7.h"
#include <iostream>
#include <stdexcept>
#include <map>

namespace set02::ex12
{

    namespace
    {
        // The unknown string provided in the challenge
        const std::string SECRET_B64 =
            "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
    }

    Oracle create_oracle()
    {
        auto key = rand_bytes(16);
        auto secret = base64_to_bytes(SECRET_B64);

        return [key, secret](const std::vector<unsigned char> &input) -> std::vector<unsigned char>
        {
            std::vector<unsigned char> data = input;
            data.insert(data.end(), secret.begin(), secret.end());
            return aes::encrypt_ecb(data, key); // Handles padding
        };
    }

    std::string break_ecb_simple(Oracle oracle)
    {
        // 1. Discover block size
        size_t block_size = 0;
        size_t initial_len = oracle({}).size();
        for (int i = 1; i < 64; ++i)
        {
            std::vector<unsigned char> inp(i, 'A');
            size_t len = oracle(inp).size();
            if (len > initial_len)
            {
                block_size = len - initial_len;
                break;
            }
        }
        if (block_size == 0)
            throw std::runtime_error("Could not determine block size");

        // 2. Verify oracle is ECB
        auto oracle_wrapper = [&](const std::vector<unsigned char> &inp) { return oracle(inp); };
        if (detect_ecb_or_cbc(oracle_wrapper) != "ECB") throw std::runtime_error("Oracle does not appear to be ECB");

        // 3. Recover the unknown string byte-by-byte
        std::string unknown_string;
        size_t unknown_len = oracle({}).size(); // Upper bound

        for (size_t i = 0; i < unknown_len; ++i)
        {

            // Recover the i-th byte via byte-at-a-time ECB attack
            size_t block_idx = i / block_size;
            size_t pad_len = block_size - 1 - (i % block_size);
            std::vector<unsigned char> prefix(pad_len, 'A');

            // Get target ciphertext block for this position
            auto cipher = oracle(prefix);
            std::vector<unsigned char> target_block(cipher.begin() + block_idx * block_size,
                                                    cipher.begin() + (block_idx + 1) * block_size);

            unsigned char found_byte = 0;
            bool found = false;

            for (int c = 0; c < 256; ++c)
            {
                std::vector<unsigned char> probe = prefix;
                probe.insert(probe.end(), unknown_string.begin(), unknown_string.end());
                probe.push_back((unsigned char)c);

                auto probe_cipher = oracle(probe);
                std::vector<unsigned char> probe_block(probe_cipher.begin() + block_idx * block_size,
                                                       probe_cipher.begin() + (block_idx + 1) * block_size);

                if (probe_block == target_block)
                {
                    found_byte = (unsigned char)c;
                    found = true;
                    break;
                }
            }

            if (found) {
                // append recovered byte
                unknown_string.push_back(found_byte);
            } else {
                // no match -> likely reached end
                break;
            }
        }

        // Strip PKCS#7 padding if present and return
        try
        {
            std::vector<unsigned char> res_bytes(unknown_string.begin(), unknown_string.end());
            auto unpadded = pkcs7::unpad(res_bytes);
            return std::string(unpadded.begin(), unpadded.end());
        }
        catch (...)
        {
            // If unpad fails (maybe we stopped early?), return as is
            return unknown_string;
        }
    }

} // namespace set02::ex12
