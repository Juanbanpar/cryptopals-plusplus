#include "ex06.h"
#include "common.h"

#include <fstream>
#include <algorithm>
#include <limits>

using namespace set01::ex06;

static double normalized_hamming_for_keysize(const std::vector<unsigned char> &data, int keysize)
{
    int blocks = std::min(4, static_cast<int>(data.size() / keysize));
    if (blocks < 2)
        return std::numeric_limits<double>::infinity();
    double total = 0.0;
    int pairs = 0;
    for (int i = 0; i + 1 < blocks; ++i)
    {
        std::vector<unsigned char> a(data.begin() + i * keysize, data.begin() + (i + 1) * keysize);
        std::vector<unsigned char> b(data.begin() + (i + 1) * keysize, data.begin() + (i + 2) * keysize);
        total += (double)hamming_distance(a, b) / (double)keysize;
        pairs++;
    }
    return total / pairs;
}

std::pair<std::string, std::string> set01::ex06::break_repeating_key_xor_from_base64(const std::string &b64)
{
    auto data = base64_to_bytes(b64);
    if (data.empty())
        return {"", ""};

    std::vector<std::pair<double, int>> scores;
    for (int keysize = 2; keysize <= 40; ++keysize)
    {
        double nh = normalized_hamming_for_keysize(data, keysize);
        scores.push_back({nh, keysize});
    }
    std::sort(scores.begin(), scores.end());

    double best_score = -std::numeric_limits<double>::infinity();
    std::string best_key;
    std::string best_plain;

    int try_count = std::min(5, (int)scores.size());
    for (int t = 0; t < try_count; ++t)
    {
        int keysize = scores[t].second;
        std::vector<std::vector<unsigned char>> blocks(keysize);
        for (size_t i = 0; i < data.size(); ++i)
            blocks[i % keysize].push_back(data[i]);

        std::string key;
        key.resize(keysize);
        for (int i = 0; i < keysize; ++i)
        {
            auto [k, plain] = crack_single_byte_xor_bytes(blocks[i]);
            key[i] = static_cast<char>(k);
        }

        std::string plain;
        plain.resize(data.size());
        for (size_t i = 0; i < data.size(); ++i)
            plain[i] = static_cast<char>(data[i] ^ (unsigned char)key[i % keysize]);

        double sc = score_english(plain);
        if (sc > best_score)
        {
            best_score = sc;
            best_key = key;
            best_plain = plain;
        }
    }

    return {best_key, best_plain};
}

std::pair<std::string, std::string> set01::ex06::break_repeating_key_xor_from_base64_file(const std::string &path)
{
    std::ifstream ifs(path);
    if (!ifs)
        return {"", ""};
    std::string line, b64;
    while (std::getline(ifs, line))
        b64 += line;
    return break_repeating_key_xor_from_base64(b64);
}
