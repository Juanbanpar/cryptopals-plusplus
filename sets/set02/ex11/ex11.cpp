#include "ex11.h"
#include "common.h"
#include "aes.h"
#include "pkcs7.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <set>

namespace set02::ex11
{

    std::vector<unsigned char> encryption_oracle(const std::vector<unsigned char> &input, std::string *out_mode)
    {
        // Random key
        auto key = rand_bytes(16);

        // Random prepend 5-10 bytes
        auto prepend_len = 5 + (rand_bytes(1)[0] % 6);
        auto prepend = rand_bytes(prepend_len);

        // Random append 5-10 bytes
        auto append_len = 5 + (rand_bytes(1)[0] % 6);
        auto append = rand_bytes(append_len);

        std::vector<unsigned char> data = prepend;
        data.insert(data.end(), input.begin(), input.end());
        data.insert(data.end(), append.begin(), append.end());

        // Choose mode: 0=ECB, 1=CBC
        int mode_bit = rand_bytes(1)[0] % 2;

        if (mode_bit == 0)
        {
            if (out_mode)
                *out_mode = "ECB";
            // Encrypt ECB
            // Note: encrypt_ecb handles PKCS#7 padding if pad=true (default)
            return aes::encrypt_ecb(data, key);
        }
        else
        {
            if (out_mode)
                *out_mode = "CBC";
            // Encrypt CBC
            auto iv = rand_bytes(16);
            return aes::encrypt_cbc(data, key, iv);
        }
    }

    std::string detect_mode(const Oracle &oracle)
    {
        // To detect ECB, we need at least 2 identical blocks in plaintext to produce 2 identical blocks in ciphertext (if aligned).
        // The prefix is 5-10 bytes. The suffix is 5-10 bytes.
        // If we send a payload of, say, 48 bytes (3 blocks) of 'A',
        //   Prefix (5-10) + 'A'*48 + Suffix (5-10)
        //   Total length is 58 to 68 bytes + padding.
        //   The 'A's will definitely cover full 16-byte blocks somewhere alignment wise.
        //   With 5-10 byte prefix, the first 'A' starts at index 5..10.
        //   Block 0: [Prefix...A...]
        //   Block 1: [A...A]  <-- This might not be fully A's if prefix is long?
        //   Wait.
        //   Prefix max 10.
        //   We want input that guarantees at least two full blocks of identical bytes are ENCRYPTED.
        //   If we send 43 bytes of 'A':
        //     Prefix (5-10) + 'A'*43
        //     Maximum prefix 10 -> 'A' starts at 10.
        //     Bytes 10..52 are 'A'. That's 42 bytes.
        //     Block 1 (16-31) is all 'A'.
        //     Block 2 (32-47) is all 'A'.
        //     Since they are identical plaintext blocks, ECB will produce identical ciphertext blocks.

        std::vector<unsigned char> payload(64, 'A');
        auto ciphertext = oracle(payload);

        // Check for repeated blocks
        std::set<std::string> blocks;
        for (size_t i = 0; i < ciphertext.size(); i += 16)
        {
            if (i + 16 > ciphertext.size())
                break;
            std::string block(ciphertext.begin() + i, ciphertext.begin() + i + 16);
            if (blocks.count(block))
            {
                return "ECB";
            }
            blocks.insert(block);
        }

        return "CBC";
    }

} // namespace set02::ex11
