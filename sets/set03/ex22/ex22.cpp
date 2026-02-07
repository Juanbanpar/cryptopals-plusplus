#include "ex22.h"
#include "../ex21/ex21.h"
#include <random>
#include <chrono>

namespace set03::ex22
{

    uint32_t generate_seeded_output(uint32_t &actual_seed)
    {
        // Simulate waiting: use current time + random offset
        auto now = std::chrono::system_clock::now();
        auto epoch = now.time_since_epoch();
        uint32_t timestamp = (uint32_t)std::chrono::duration_cast<std::chrono::seconds>(epoch).count();

        // Simulate random wait (40-1000 seconds in past)
        // I use std::mt19937 instead of ex21 so tests are independent of ex21's implementation
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dist(40, 1000);
        uint32_t wait1 = dist(gen);
        uint32_t wait2 = dist(gen);

        actual_seed = timestamp - wait1 - wait2;

        set03::ex21::MT19937 rng(actual_seed);
        return rng();
    }

    uint32_t crack_seed(uint32_t output, uint32_t time_now, uint32_t max_seconds_back)
    {
        // Try seeds from recent past
        for (uint32_t offset = 0; offset <= max_seconds_back; ++offset)
        {
            uint32_t candidate_seed = time_now - offset;
            set03::ex21::MT19937 rng(candidate_seed);
            if (rng() == output)
                return candidate_seed;
        }
        return 0; // Not found
    }

} // namespace set03::ex22
