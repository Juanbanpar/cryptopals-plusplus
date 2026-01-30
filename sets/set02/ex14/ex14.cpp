#include "ex14.h"
#include "common.h"
#include "aes.h"
#include "pkcs7.h"
#include <stdexcept>
#include <iostream>
#include <random>

namespace set02::ex14
{

    namespace
    {
        const std::string SECRET_B64 =
            "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
    }

    Oracle create_oracle()
    {
        auto key = rand_bytes(16);
        auto secret = base64_to_bytes(SECRET_B64);

        // Random prefix (length 1..100) fixed for this oracle instance
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);
        size_t prefix_len = dis(gen);
        auto prefix = rand_bytes(prefix_len);

        return [key, secret, prefix](const std::vector<unsigned char> &input) -> std::vector<unsigned char>
        {
            std::vector<unsigned char> data = prefix;
            data.insert(data.end(), input.begin(), input.end());
            data.insert(data.end(), secret.begin(), secret.end());
            return aes::encrypt_ecb(data, key);
        };
    }

    std::string break_ecb_harder(Oracle oracle)
    {
        // 1. Find prefix length by adding 0..15 pad bytes plus 2 matching blocks
        // and searching ciphertext for two identical adjacent blocks.
        size_t block_size = 16;
        size_t prefix_len = 0;
        size_t needed_pad = 0;
        size_t prefix_block_count = 0;
        bool found_prefix = false;

        for (size_t p = 0; p < block_size; ++p) {
            std::vector<unsigned char> inp(p + 2 * block_size, 'A');
            auto cipher = oracle(inp);
            for (size_t i = 0; i + 1 < cipher.size() / block_size; ++i) {
                std::vector<unsigned char> b1(cipher.begin() + i * block_size, cipher.begin() + (i + 1) * block_size);
                std::vector<unsigned char> b2(cipher.begin() + (i + 1) * block_size, cipher.begin() + (i + 2) * block_size);
                if (b1 == b2) {
                    // i*block_size = prefix_len + p  => prefix_len = i*block_size - p
                    prefix_block_count = i;
                    needed_pad = p;
                    prefix_len = i * block_size - p;
                    found_prefix = true;
                    break;
                }
            }
            if (found_prefix) break;
        }
        if (!found_prefix) throw std::runtime_error("Failed to detect prefix length");

        // 2. Decrypt: treat prefix+needed_pad as an offset, then perform byte-at-a-time
        // For each secret byte: send (needed_pad + pad_len) 'A's so the target byte aligns
        // at the end of a block after the prefix. Compare blocks accounting for prefix_block_count.
        std::string unknown_string;
        size_t unknown_len = oracle({}).size() - prefix_len;
        for (size_t i = 0; i < unknown_len; ++i) {
            size_t block_idx = i / block_size;
            size_t pad_len = block_size - 1 - (i % block_size);

            std::vector<unsigned char> inp(needed_pad + pad_len, 'A');
            auto cipher = oracle(inp);

            size_t actual_block_idx = prefix_block_count + block_idx;
            if ((actual_block_idx + 1) * block_size > cipher.size()) break;

            std::vector<unsigned char> target_block(cipher.begin() + actual_block_idx * block_size,
                                                    cipher.begin() + (actual_block_idx + 1) * block_size);

            unsigned char found_byte = 0;
            bool found = false;
            for (int c = 0; c < 256; ++c) {
                std::vector<unsigned char> probe = inp;
                probe.insert(probe.end(), unknown_string.begin(), unknown_string.end());
                probe.push_back((unsigned char)c);

                auto probe_cipher = oracle(probe);
                std::vector<unsigned char> probe_block(probe_cipher.begin() + actual_block_idx * block_size,
                                                       probe_cipher.begin() + (actual_block_idx + 1) * block_size);
                if (probe_block == target_block) { found_byte = (unsigned char)c; found = true; break; }
            }
            if (found) unknown_string.push_back(found_byte); else break;
        }

        try
        {
            std::vector<unsigned char> res_bytes(unknown_string.begin(), unknown_string.end());
            auto unpadded = pkcs7::unpad(res_bytes);
            return std::string(unpadded.begin(), unpadded.end());
        }
        catch (...)
        {
            return unknown_string;
        }
    }

} // namespace set02::ex14