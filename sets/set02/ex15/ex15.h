#pragma once

#include <vector>
#include <string>

namespace set02::ex15
{

    std::vector<unsigned char> validate_and_strip_padding(const std::vector<unsigned char> &input);
    std::string validate_and_strip_padding_str(const std::string &input);

} // namespace set02::ex15
