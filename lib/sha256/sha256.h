#pragma once

#include <vector>

namespace sha256
{

    std::vector<unsigned char> digest(const std::vector<unsigned char> &data);

} // namespace sha256
