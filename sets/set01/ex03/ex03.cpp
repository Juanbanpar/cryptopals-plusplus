#include "ex03.h"
#include "common.h"

namespace set01::ex03
{

    std::pair<unsigned char, std::string> crack_single_byte_xor(const std::string &hex)
    {
        return ::crack_single_byte_xor_hex(hex);
    }

} // namespace set01::ex03
static double table[256];
