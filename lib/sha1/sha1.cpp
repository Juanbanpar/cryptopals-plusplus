#include "sha1.h"
#include <stdexcept>

namespace sha1
{

    namespace
    {
        constexpr uint32_t INITIAL_A = 0x67452301;
        constexpr uint32_t INITIAL_B = 0xefcdab89;
        constexpr uint32_t INITIAL_C = 0x98badcfe;
        constexpr uint32_t INITIAL_D = 0x10325476;
        constexpr uint32_t INITIAL_E = 0xc3d2e1f0;

        uint32_t left_rotate(uint32_t value, unsigned int amount)
        {
            return (value << amount) | (value >> (32 - amount));
        }

        uint32_t read_big_endian(const unsigned char *data)
        {
            return (static_cast<uint32_t>(data[0]) << 24) |
                   (static_cast<uint32_t>(data[1]) << 16) |
                   (static_cast<uint32_t>(data[2]) << 8) |
                   static_cast<uint32_t>(data[3]);
        }

        void append_big_endian_word(std::vector<unsigned char> &out, uint32_t value)
        {
            for (int shift = 24; shift >= 0; shift -= 8)
                out.push_back(static_cast<unsigned char>(value >> shift));
        }

        void append_big_endian_length(std::vector<unsigned char> &out, uint64_t value)
        {
            for (int shift = 56; shift >= 0; shift -= 8)
                out.push_back(static_cast<unsigned char>(value >> shift));
        }

        void process_block(const unsigned char *block, State &state)
        {
            uint32_t words[80] = {};
            for (size_t i = 0; i < 16; ++i)
                words[i] = read_big_endian(block + i * 4);

            for (size_t i = 16; i < 80; ++i)
            {
                words[i] = left_rotate(
                    words[i - 3] ^ words[i - 8] ^ words[i - 14] ^ words[i - 16],
                    1
                );
            }

            uint32_t a = state[0];
            uint32_t b = state[1];
            uint32_t c = state[2];
            uint32_t d = state[3];
            uint32_t e = state[4];

            for (size_t i = 0; i < 80; ++i)
            {
                uint32_t function;
                uint32_t constant;

                if (i < 20)
                {
                    function = (b & c) | ((~b) & d);
                    constant = 0x5a827999;
                }
                else if (i < 40)
                {
                    function = b ^ c ^ d;
                    constant = 0x6ed9eba1;
                }
                else if (i < 60)
                {
                    function = (b & c) | (b & d) | (c & d);
                    constant = 0x8f1bbcdc;
                }
                else
                {
                    function = b ^ c ^ d;
                    constant = 0xca62c1d6;
                }

                const uint32_t next = left_rotate(a, 5) + function + e + constant + words[i];
                e = d;
                d = c;
                c = left_rotate(b, 30);
                b = a;
                a = next;
            }

            state[0] += a;
            state[1] += b;
            state[2] += c;
            state[3] += d;
            state[4] += e;
        }

        State initial_state()
        {
            return {INITIAL_A, INITIAL_B, INITIAL_C, INITIAL_D, INITIAL_E};
        }
    }

    std::vector<unsigned char> glue_padding(size_t message_length)
    {
        const uint64_t length = static_cast<uint64_t>(message_length);
        std::vector<unsigned char> padding{0x80};
        const size_t zeroes = (56 - ((message_length + 1) % 64)) % 64;
        padding.insert(padding.end(), zeroes, 0);
        append_big_endian_length(padding, length * 8);
        return padding;
    }

    std::vector<unsigned char> digest_with_state(
        const std::vector<unsigned char> &data,
        const State &state,
        uint64_t processed_bytes
    )
    {
        if (processed_bytes % 64 != 0)
            throw std::invalid_argument("SHA-1 processed length must be block aligned");

        const uint64_t total_length = processed_bytes + data.size();
        auto padded = data;
        const auto padding = glue_padding(static_cast<size_t>(total_length));
        padded.insert(padded.end(), padding.begin(), padding.end());

        State result = state;
        for (size_t i = 0; i < padded.size(); i += 64)
            process_block(padded.data() + i, result);

        std::vector<unsigned char> output;
        output.reserve(20);
        for (uint32_t word : result)
            append_big_endian_word(output, word);
        return output;
    }

    std::vector<unsigned char> digest(const std::vector<unsigned char> &data)
    {
        return digest_with_state(data, initial_state(), 0);
    }

    State state_from_digest(const std::vector<unsigned char> &digest_value)
    {
        if (digest_value.size() != 20)
            throw std::invalid_argument("SHA-1 digest must be 20 bytes");

        State state{};
        for (size_t i = 0; i < state.size(); ++i)
        {
            state[i] = (static_cast<uint32_t>(digest_value[i * 4]) << 24) |
                       (static_cast<uint32_t>(digest_value[i * 4 + 1]) << 16) |
                       (static_cast<uint32_t>(digest_value[i * 4 + 2]) << 8) |
                       static_cast<uint32_t>(digest_value[i * 4 + 3]);
        }
        return state;
    }

} // namespace sha1
