#include "ex21.h"
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <seed>" << std::endl;
        return 1;
    }

    unsigned long seed;
    try
    {
        seed = std::stoul(argv[1]);
    }
    catch (const std::exception&)
    {
        std::cerr << "Invalid seed: " << argv[1] << std::endl;
        return 1;
    }

    set03::ex21::MT19937 rng(static_cast<uint32_t>(seed));

    std::cout << "First 10 outputs from MT19937 (seed=" << seed << "):" << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << rng() << std::endl;
    }

    return 0;
}
