#pragma once
#include <cstdint>

namespace set03::ex22
{

    // Simulate waiting and seeding with timestamp, returns first output
    uint32_t generate_seeded_output(uint32_t &actual_seed);

    // Crack the seed from a single output and approximate time range
    uint32_t crack_seed(uint32_t output, uint32_t time_now, uint32_t max_seconds_back = 2000);

} // namespace set03::ex22
