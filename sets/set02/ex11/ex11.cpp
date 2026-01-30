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

    // detect_mode was removed; use detect_ecb_or_cbc from lib/common instead.

} // namespace set02::ex11
