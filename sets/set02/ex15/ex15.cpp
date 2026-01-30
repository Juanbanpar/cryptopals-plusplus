#include "ex15.h"
#include "pkcs7.h"

namespace set02::ex15
{

    std::vector<unsigned char> validate_and_strip_padding(const std::vector<unsigned char> &input)
    {
        // This library function already validates
        return pkcs7::unpad(input);
    }

    std::string validate_and_strip_padding_str(const std::string &input)
    {
        std::vector<unsigned char> v(input.begin(), input.end());
        auto res = validate_and_strip_padding(v);
        return std::string(res.begin(), res.end());
    }

} // namespace set02::ex15
