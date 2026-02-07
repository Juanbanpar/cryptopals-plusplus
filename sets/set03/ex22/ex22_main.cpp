#include "ex22.h"
#include <iostream>
#include <chrono>

int main()
{
    uint32_t actual_seed;
    uint32_t output = set03::ex22::generate_seeded_output(actual_seed);

    auto now = std::chrono::system_clock::now();
    auto epoch = now.time_since_epoch();
    uint32_t time_now = (uint32_t)std::chrono::duration_cast<std::chrono::seconds>(epoch).count();

    std::cout << "First MT19937 output: " << output << std::endl;
    std::cout << "Cracking seed..." << std::endl;

    uint32_t cracked = set03::ex22::crack_seed(output, time_now, 2500);

    std::cout << "Actual seed:  " << actual_seed << std::endl;
    std::cout << "Cracked seed: " << cracked << std::endl;

    if (cracked == actual_seed)
        std::cout << "SUCCESS: Seed cracked!" << std::endl;
    else
        std::cout << "FAILURE: Could not crack seed" << std::endl;

    return (cracked == actual_seed) ? 0 : 1;
}
