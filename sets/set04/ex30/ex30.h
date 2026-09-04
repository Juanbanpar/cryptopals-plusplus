#pragma once

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace set04::ex30
{

    using MacOracle = std::function<bool(
        const std::vector<unsigned char> &message,
        const std::vector<unsigned char> &mac
    )>;

    // Create a secret-prefix MD4 MAC: MD4(key || message).
    std::vector<unsigned char> md4_mac(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message
    );

    // Verify a secret-prefix MD4 MAC.
    bool verify_md4_mac(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message,
        const std::vector<unsigned char> &mac
    );

    // Forge an MD4 secret-prefix MAC by guessing the secret key length.
    std::pair<std::vector<unsigned char>, std::vector<unsigned char>> forge_md4_mac(
        const std::vector<unsigned char> &original_message,
        const std::vector<unsigned char> &original_mac,
        const std::vector<unsigned char> &append_data,
        const MacOracle &oracle,
        size_t min_key_length = 0,
        size_t max_key_length = 64
    );

} // namespace set04::ex30
