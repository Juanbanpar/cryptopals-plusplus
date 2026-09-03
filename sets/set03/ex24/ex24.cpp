#include "ex24.h"
#include "../ex21/ex21.h"
#include "common.h"
#include <algorithm>
#include <chrono>
#include <random>

namespace set03::ex24
{

namespace
{
    uint32_t current_timestamp()
    {
        const auto now = std::chrono::system_clock::now();
        const auto epoch = now.time_since_epoch();
        return static_cast<uint32_t>(
            std::chrono::duration_cast<std::chrono::seconds>(epoch).count());
    }

    unsigned char next_keystream_byte(set03::ex21::MT19937 &rng)
    {
        // The challenge asks for an 8-bit keystream. Use one byte from each
        // 32-bit MT19937 output, matching the byte-oriented cipher API.
        return static_cast<unsigned char>(rng() & 0xffu);
    }

    bool matches_token(const std::vector<unsigned char> &token, uint32_t seed)
    {
        set03::ex21::MT19937 rng(seed);
        for (unsigned char expected : token)
        {
            if (next_keystream_byte(rng) != expected)
                return false;
        }
        return true;
    }
}

std::vector<unsigned char> mt19937_cipher(const std::vector<unsigned char> &data, uint16_t seed)
{
    set03::ex21::MT19937 rng(seed);
    std::vector<unsigned char> result(data.size());

    for (size_t i = 0; i < data.size(); ++i)
        result[i] = data[i] ^ next_keystream_byte(rng);

    return result;
}

std::vector<unsigned char> encrypt_with_random_prefix(const std::vector<unsigned char> &known_plaintext, uint16_t seed)
{
    // The challenge uses a random prefix between 5 and 20 bytes.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(5, 20);
    const size_t prefix_len = dist(gen);

    const auto prefix = rand_bytes(prefix_len);
    std::vector<unsigned char> plaintext;
    plaintext.reserve(prefix.size() + known_plaintext.size());
    plaintext.insert(plaintext.end(), prefix.begin(), prefix.end());
    plaintext.insert(plaintext.end(), known_plaintext.begin(), known_plaintext.end());

    return mt19937_cipher(plaintext, seed);
}

uint16_t recover_seed(const std::vector<unsigned char> &ciphertext, const std::vector<unsigned char> &known_plaintext)
{
    // There is no useful seed recovery without a non-empty known suffix.
    if (known_plaintext.empty() || ciphertext.size() < known_plaintext.size())
        return 0;

    // The known plaintext is at the end of the message.
    const size_t known_len = known_plaintext.size();
    const size_t prefix_len = ciphertext.size() - known_len;

    // Brute force all 16-bit seeds.
    for (uint32_t seed = 0; seed <= 0xffff; ++seed)
    {
        const auto decrypted = mt19937_cipher(ciphertext, static_cast<uint16_t>(seed));

        bool match = true;
        for (size_t i = 0; i < known_len; ++i)
        {
            if (decrypted[prefix_len + i] != known_plaintext[i])
            {
                match = false;
                break;
            }
        }

        if (match)
            return static_cast<uint16_t>(seed);
    }

    // Keep the existing uint16_t API’s failure convention. Seed zero is
    // still recoverable when it is the first matching candidate.
    return 0;
}

std::vector<unsigned char> generate_password_token()
{
    set03::ex21::MT19937 rng(current_timestamp());

    // Generate a 16-byte token.
    std::vector<unsigned char> token(16);
    for (size_t i = 0; i < token.size(); ++i)
        token[i] = next_keystream_byte(rng);

    return token;
}

bool is_mt19937_time_token(const std::vector<unsigned char> &token, uint32_t max_seconds_back)
{
    if (token.empty())
        return false;

    const uint32_t time_now = current_timestamp();
    const uint64_t seconds_to_check = std::min<uint64_t>(max_seconds_back, time_now);

    for (uint64_t offset = 0; offset <= seconds_to_check; ++offset)
    {
        const uint32_t candidate_seed = static_cast<uint32_t>(time_now - offset);
        if (matches_token(token, candidate_seed))
            return true;
    }

    return false;
}

} // namespace set03::ex24
