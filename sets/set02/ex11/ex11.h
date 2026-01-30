#pragma once

#include <vector>
#include <string>
#include <functional>

namespace set02::ex11
{

    // The oracle function type
    using Oracle = std::function<std::vector<unsigned char>(const std::vector<unsigned char> &)>;

    // Generates a ciphertext using random key/IV, random padding, and randomly choosing ECB or CBC.
    // Note: This function allows checking the used mode via the optional out_mode parameter for verification.
    std::vector<unsigned char> encryption_oracle(const std::vector<unsigned char> &input, std::string *out_mode = nullptr);

    // (detection moved to lib/common: detect_ecb_or_cbc)

} // namespace set02::ex11
