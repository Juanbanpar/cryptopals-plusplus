#include "ex18.h"
#include "common.h"
#include "aes.h"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <text> <key>\n";
        return 2;
    }

    std::string plaintext = argv[1];
    std::string key_str = "YELLOW SUBMARINE";
    if (argc > 2)
        key_str = argv[2];

    // 16-byte key (will be truncated or zero-padded to 16 bytes)
    if (key_str.size() < 16)
        key_str.resize(16, '\0');
    else if (key_str.size() > 16)
        key_str = key_str.substr(0, 16);

    std::vector<uint8_t> key(key_str.begin(), key_str.end());
    uint64_t nonce = 0;

    std::vector<uint8_t> pt_bytes(plaintext.begin(), plaintext.end());
    auto ciphertext = aes::ctr(pt_bytes, key, nonce);

    std::cout << "Output (hex): " << bytes_to_hex(ciphertext) << std::endl;
    std::cout << "Output (base64): " << bytes_to_base64(ciphertext) << std::endl;

    return 0;
}
