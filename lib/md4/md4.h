#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace md4
{

    using State = std::array<uint32_t, 4>;

    std::vector<unsigned char> digest(const std::vector<unsigned char> &data);

    std::vector<unsigned char> digest_with_state(
        const std::vector<unsigned char> &data,
        const State &state,
        uint64_t processed_bytes
    );

    std::vector<unsigned char> glue_padding(size_t message_length);

    State state_from_digest(const std::vector<unsigned char> &digest);

} // namespace md4
