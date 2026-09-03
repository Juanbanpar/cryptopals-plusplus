#pragma once

#include <cstdint>
#include <vector>

namespace set03::ex21
{

    // MT19937 Mersenne Twister implementation
    class MT19937
    {
    public:
        MT19937(uint32_t seed = 5489);

        // Seed the generator
        void seed(uint32_t s);

        // Generate next random number
        uint32_t operator()();

        // Get the internal state (for cloning attack)
        const std::vector<uint32_t> &get_state() const { return mt; }

        // Set the internal state (for cloning attack)
        void set_state(const std::vector<uint32_t> &state, size_t idx = 0);

    private:
        static constexpr size_t N = 624;
        static constexpr size_t M = 397;
        static constexpr uint32_t MATRIX_A = 0x9908b0df;
        static constexpr uint32_t UPPER_MASK = 0x80000000;
        static constexpr uint32_t LOWER_MASK = 0x7fffffff;

        std::vector<uint32_t> mt;
        size_t mti;

        void twist();
    };

} // namespace set03::ex21
