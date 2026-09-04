#include "md4.h"
#include <stdexcept>

namespace md4
{

    namespace
    {
        constexpr uint32_t INITIAL_A = 0x67452301;
        constexpr uint32_t INITIAL_B = 0xefcdab89;
        constexpr uint32_t INITIAL_C = 0x98badcfe;
        constexpr uint32_t INITIAL_D = 0x10325476;

        uint32_t left_rotate(uint32_t value, unsigned int amount)
        {
            return (value << amount) | (value >> (32 - amount));
        }

        uint32_t read_little_endian(const unsigned char *data)
        {
            return static_cast<uint32_t>(data[0]) |
                   (static_cast<uint32_t>(data[1]) << 8) |
                   (static_cast<uint32_t>(data[2]) << 16) |
                   (static_cast<uint32_t>(data[3]) << 24);
        }

        void append_little_endian_word(std::vector<unsigned char> &out, uint32_t value)
        {
            for (int shift = 0; shift < 32; shift += 8)
                out.push_back(static_cast<unsigned char>(value >> shift));
        }

        void append_little_endian_length(std::vector<unsigned char> &out, uint64_t value)
        {
            for (int shift = 0; shift < 64; shift += 8)
                out.push_back(static_cast<unsigned char>(value >> shift));
        }

        uint32_t f(uint32_t x, uint32_t y, uint32_t z)
        {
            return (x & y) | ((~x) & z);
        }

        uint32_t g(uint32_t x, uint32_t y, uint32_t z)
        {
            return (x & y) | (x & z) | (y & z);
        }

        uint32_t h(uint32_t x, uint32_t y, uint32_t z)
        {
            return x ^ y ^ z;
        }

        void process_block(const unsigned char *block, State &state)
        {
            uint32_t words[16] = {};
            for (size_t i = 0; i < 16; ++i)
                words[i] = read_little_endian(block + i * 4);

            uint32_t a = state[0];
            uint32_t b = state[1];
            uint32_t c = state[2];
            uint32_t d = state[3];

            const unsigned int round_one_shifts[4] = {3, 7, 11, 19};
            for (size_t i = 0; i < 16; ++i)
            {
                const uint32_t value = f(b, c, d) + words[i];
                switch (i % 4)
                {
                case 0:
                    a = left_rotate(a + value, round_one_shifts[i % 4]);
                    break;
                case 1:
                    d = left_rotate(d + f(a, b, c) + words[i], round_one_shifts[i % 4]);
                    break;
                case 2:
                    c = left_rotate(c + f(d, a, b) + words[i], round_one_shifts[i % 4]);
                    break;
                default:
                    b = left_rotate(b + f(c, d, a) + words[i], round_one_shifts[i % 4]);
                    break;
                }
            }

            const size_t round_two_order[16] = {
                0, 4, 8, 12, 1, 5, 9, 13,
                2, 6, 10, 14, 3, 7, 11, 15};
            const unsigned int round_two_shifts[4] = {3, 5, 9, 13};
            for (size_t i = 0; i < 16; ++i)
            {
                const uint32_t word = words[round_two_order[i]] + 0x5a827999;
                switch (i % 4)
                {
                case 0:
                    a = left_rotate(a + g(b, c, d) + word, round_two_shifts[i % 4]);
                    break;
                case 1:
                    d = left_rotate(d + g(a, b, c) + word, round_two_shifts[i % 4]);
                    break;
                case 2:
                    c = left_rotate(c + g(d, a, b) + word, round_two_shifts[i % 4]);
                    break;
                default:
                    b = left_rotate(b + g(c, d, a) + word, round_two_shifts[i % 4]);
                    break;
                }
            }

            const size_t round_three_order[16] = {
                0, 8, 4, 12, 2, 10, 6, 14,
                1, 9, 5, 13, 3, 11, 7, 15};
            const unsigned int round_three_shifts[4] = {3, 9, 11, 15};
            for (size_t i = 0; i < 16; ++i)
            {
                const uint32_t word = words[round_three_order[i]] + 0x6ed9eba1;
                switch (i % 4)
                {
                case 0:
                    a = left_rotate(a + h(b, c, d) + word, round_three_shifts[i % 4]);
                    break;
                case 1:
                    d = left_rotate(d + h(a, b, c) + word, round_three_shifts[i % 4]);
                    break;
                case 2:
                    c = left_rotate(c + h(d, a, b) + word, round_three_shifts[i % 4]);
                    break;
                default:
                    b = left_rotate(b + h(c, d, a) + word, round_three_shifts[i % 4]);
                    break;
                }
            }

            state[0] += a;
            state[1] += b;
            state[2] += c;
            state[3] += d;
        }

        State initial_state()
        {
            return {INITIAL_A, INITIAL_B, INITIAL_C, INITIAL_D};
        }
    }

    std::vector<unsigned char> glue_padding(size_t message_length)
    {
        const uint64_t length = static_cast<uint64_t>(message_length);
        std::vector<unsigned char> padding{0x80};
        const size_t zeroes = (56 - ((message_length + 1) % 64)) % 64;
        padding.insert(padding.end(), zeroes, 0);
        append_little_endian_length(padding, length * 8);
        return padding;
    }

    std::vector<unsigned char> digest_with_state(
        const std::vector<unsigned char> &data,
        const State &state,
        uint64_t processed_bytes
    )
    {
        if (processed_bytes % 64 != 0)
            throw std::invalid_argument("MD4 processed length must be block aligned");

        const uint64_t total_length = processed_bytes + data.size();
        auto padded = data;
        const auto padding = glue_padding(static_cast<size_t>(total_length));
        padded.insert(padded.end(), padding.begin(), padding.end());

        State result = state;
        for (size_t i = 0; i < padded.size(); i += 64)
            process_block(padded.data() + i, result);

        std::vector<unsigned char> output;
        output.reserve(16);
        for (uint32_t word : result)
            append_little_endian_word(output, word);
        return output;
    }

    std::vector<unsigned char> digest(const std::vector<unsigned char> &data)
    {
        return digest_with_state(data, initial_state(), 0);
    }

    State state_from_digest(const std::vector<unsigned char> &digest_value)
    {
        if (digest_value.size() != 16)
            throw std::invalid_argument("MD4 digest must be 16 bytes");

        State state{};
        for (size_t i = 0; i < state.size(); ++i)
        {
            state[i] = static_cast<uint32_t>(digest_value[i * 4]) |
                       (static_cast<uint32_t>(digest_value[i * 4 + 1]) << 8) |
                       (static_cast<uint32_t>(digest_value[i * 4 + 2]) << 16) |
                       (static_cast<uint32_t>(digest_value[i * 4 + 3]) << 24);
        }
        return state;
    }

} // namespace md4
