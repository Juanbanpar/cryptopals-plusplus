#pragma once

#include <vector>

namespace set04::ex28
{

    // Create a secret-prefix SHA-1 MAC: SHA1(key || message).
    std::vector<unsigned char> sha1_mac(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message
    );

    // Verify a secret-prefix SHA-1 MAC.
    bool verify_sha1_mac(
        const std::vector<unsigned char> &key,
        const std::vector<unsigned char> &message,
        const std::vector<unsigned char> &mac
    );

} // namespace set04::ex28
