#pragma once
#include <vector>
#include <string>
#include <functional>

namespace set02::ex14
{

    using Oracle = std::function<std::vector<unsigned char>(const std::vector<unsigned char> &)>;

    // Encrypts: AES-128-ECB(random-prefix || input || secret, key)
    Oracle create_oracle();

    // Break it
    std::string break_ecb_harder(Oracle oracle);

} // namespace set02::ex14
