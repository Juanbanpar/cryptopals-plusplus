#include "ex07.h"
#include "common.h"
#include "aes.h"

#include <fstream>
#include <vector>

namespace {}

std::string set01::ex07::decrypt_aes_128_ecb_base64(const std::vector<unsigned char> &data, const std::string &key)
{
    std::vector<unsigned char> key_bytes(key.begin(), key.end());
    // AES library returns vector, we convert to string.
    // Note: aes::decrypt_ecb expects padded input and unpads by default.
    auto plain_bytes = aes::decrypt_ecb(data, key_bytes);
    return std::string(plain_bytes.begin(), plain_bytes.end());
}

std::string set01::ex07::decrypt_aes_128_ecb_base64_file(const std::string &path, const std::string &key)
{
    std::ifstream ifs(path);
    if (!ifs)
        return std::string();
    std::string line, b64;
    while (std::getline(ifs, line))
        b64 += line;
    auto cipher = base64_to_bytes(b64);
    return decrypt_aes_128_ecb_base64(cipher, key);
}
