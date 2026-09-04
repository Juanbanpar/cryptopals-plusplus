#pragma once

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace set04::ex29
{

    using MacOracle = std::function<bool(
        const std::vector<unsigned char> &message,
        const std::vector<unsigned char> &mac
    )>;

    // Forge a SHA-1 secret-prefix MAC by guessing the secret key length.
    std::pair<std::vector<unsigned char>, std::vector<unsigned char>> forge_sha1_mac(
        const std::vector<unsigned char> &original_message,
        const std::vector<unsigned char> &original_mac,
        const std::vector<unsigned char> &append_data,
        const MacOracle &oracle,
        size_t min_key_length = 0,
        size_t max_key_length = 64
    );

} // namespace set04::ex29
