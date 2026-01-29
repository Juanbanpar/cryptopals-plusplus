#include "ex09.h"
#include "pkcs7.h"

namespace set02::ex09
{

    std::vector<unsigned char> pkcs7_pad(const std::vector<unsigned char> &data, size_t block_size)
    {
        return pkcs7::pad(data, block_size);
    }

    std::string pkcs7_pad_string(const std::string &s, size_t block_size)
    {
        return pkcs7::pad_string(s, block_size);
    }

} // namespace set02::ex09
