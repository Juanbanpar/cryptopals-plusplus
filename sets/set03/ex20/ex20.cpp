#include "ex20.h"
#include "common.h"
#include "aes.h"
#include <algorithm>
#include <fstream>

namespace set03::ex20
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
    }

    std::vector<std::vector<unsigned char>> encrypt_plaintexts(const std::string &path)
    {
        ensure_init();
        std::vector<std::vector<unsigned char>> ciphertexts;

        std::ifstream ifs(path);
        if (!ifs)
            return ciphertexts;

        std::string line;
        while (std::getline(ifs, line))
        {
            if (line.empty())
                continue;
            auto plaintext = base64_to_bytes(line);
            std::vector<uint8_t> pt(plaintext.begin(), plaintext.end());
            auto ct = aes::ctr(pt, KEY, 0); // Fixed nonce = 0
            ciphertexts.push_back(std::vector<unsigned char>(ct.begin(), ct.end()));
        }

        return ciphertexts;
    }

    std::vector<std::vector<unsigned char>> encrypt_plaintexts()
    {
        std::string path = std::string(PROJECT_SOURCE_DIR) + "/resources/set03/ex20/20.txt";
        return encrypt_plaintexts(path);
    }

    std::vector<std::string> break_fixed_nonce_ctr(const std::vector<std::vector<unsigned char>> &ciphertexts)
    {
        if (ciphertexts.empty())
            return {};

        // Truncate to minimum length (as per challenge instructions)
        size_t min_len = ciphertexts[0].size();
        for (const auto &ct : ciphertexts)
            min_len = std::min(min_len, ct.size());

        // Treat as repeating-key XOR with key length = min_len
        // Transpose: group all bytes at same position
        std::vector<unsigned char> keystream(min_len, 0);

        for (size_t pos = 0; pos < min_len; ++pos)
        {
            std::vector<unsigned char> column;
            for (const auto &ct : ciphertexts)
                column.push_back(ct[pos]);

            // Find best keystream byte using single-byte XOR attack
            auto [best_key, _] = crack_single_byte_xor_bytes(column);
            keystream[pos] = best_key;
        }

        // Decrypt all ciphertexts (only up to min_len)
        std::vector<std::string> plaintexts;
        for (const auto &ct : ciphertexts)
        {
            std::string pt;
            for (size_t i = 0; i < min_len; ++i)
                pt += (char)(ct[i] ^ keystream[i]);
            plaintexts.push_back(pt);
        }

        return plaintexts;
    }

} // namespace set03::ex20
