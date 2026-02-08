#pragma once

#include <string>
#include <vector>

namespace pkcs7
{
    std::vector<unsigned char> pad(const std::vector<unsigned char> &data, size_t block_size);
    std::string pad_string(const std::string &s, size_t block_size);
    std::vector<unsigned char> unpad(const std::vector<unsigned char> &data);
}
