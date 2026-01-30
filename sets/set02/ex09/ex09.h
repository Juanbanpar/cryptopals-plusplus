#pragma once

#include <string>
#include <vector>

namespace set02::ex09
{
    std::vector<unsigned char> pkcs7_pad(const std::vector<unsigned char> &data, size_t block_size);
    std::string pkcs7_pad_string(const std::string &s, size_t block_size);
} // namespace set02::ex09
