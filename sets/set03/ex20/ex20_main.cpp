#include "ex20.h"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file_with_base64_plaintexts>" << std::endl;
        return 1;
    }

    std::string path = argv[1];
    auto ciphertexts = set03::ex20::encrypt_plaintexts(path);
    auto plaintexts = set03::ex20::break_fixed_nonce_ctr(ciphertexts);

    std::cout << "Recovered plaintexts (truncated to common length):" << std::endl;
    for (size_t i = 0; i < plaintexts.size(); ++i)
    {
        std::cout << i << ": " << plaintexts[i] << std::endl;
    }
    return 0;
}
