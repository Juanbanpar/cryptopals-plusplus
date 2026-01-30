#include "ex10.h"
#include "common.h"
#include "aes.h"
#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace set02::ex10
{

    std::string decrypt_aes_128_cbc(const std::vector<unsigned char> &cipher, const std::string &key, const std::vector<unsigned char> &iv)
    {
        // aes::decrypt_cbc returns a vector<uint8_t> and handles PKCS#7 unpadding.
        auto plain = aes::decrypt_cbc(cipher, std::vector<uint8_t>(key.begin(), key.end()), iv);
        return std::string(plain.begin(), plain.end());
    }

} // namespace set02::ex10
