#include "ex05.h"
#include "common.h"

namespace set01::ex05
{

    std::string repeating_key_xor_hex_from_text(const std::string &text, const std::string &key)
    {
        if (key.empty())
            return std::string();
        std::vector<unsigned char> out;
        out.reserve(text.size());
        for (size_t i = 0; i < text.size(); ++i)
        {
            unsigned char tb = static_cast<unsigned char>(text[i]);
            unsigned char kb = static_cast<unsigned char>(key[i % key.size()]);
            out.push_back(static_cast<unsigned char>(tb ^ kb));
        }
        return bytes_to_hex(out);
    }

} // namespace set01::ex05
