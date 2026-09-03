#include "ex19.h"
#include "common.h"
#include <iostream>
#include <fstream>


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_with_base64_ciphertexts>\n";
        return 2;
    }

    std::vector<std::vector<unsigned char>> ciphertexts;

    // Read base64 ciphertexts from provided file (one per line)
    std::string filename = argv[1];
    std::ifstream ifs(filename);
    if (!ifs)
    {
        std::cerr << "Could not open file: " << filename << std::endl;
        return 2;
    }

    std::string line;
    while (std::getline(ifs, line))
    {
        if (line.empty()) continue;
        auto bytes = base64_to_bytes(line);
        ciphertexts.push_back(std::vector<unsigned char>(bytes.begin(), bytes.end()));
    }

    auto plaintexts = set03::ex19::break_fixed_nonce_ctr(ciphertexts);

    std::cout << "Recovered plaintexts:" << std::endl;
    for (size_t i = 0; i < plaintexts.size(); ++i)
    {
        std::cout << i << ": " << plaintexts[i] << std::endl;
    }
    return 0;
}
