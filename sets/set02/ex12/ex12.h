#pragma once

#include <vector>
#include <string>
#include <functional>

namespace set02::ex12
{

    // The oracle interface: input -> ciphertext
    using Oracle = std::function<std::vector<unsigned char>(const std::vector<unsigned char> &)>;

    // Creates an oracle with a consistent, random unknown key.
    // It encrypts: AES-128-ECB(input || secret, key)
    Oracle create_oracle();

    // Decrypts the secret string from the oracle using byte-at-a-time ECB decryption.
    std::string break_ecb_simple(Oracle oracle);

} // namespace set02::ex12
