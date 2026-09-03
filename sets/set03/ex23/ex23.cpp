#include "ex23.h"
#include <vector>

namespace set03::ex23
{

namespace
{
    // Invert y ^= (y >> shift). The inverse is a finite XOR series because
    // shifting a 32-bit value by 32 bits or more produces zero.
    uint32_t unshift_right_xor(uint32_t value, unsigned shift)
    {
        uint32_t result = value;
        for (unsigned amount = shift; amount < 32; amount += shift)
            result ^= value >> amount;
        return result;
    }

    // Invert y ^= ((y << shift) & mask) by repeatedly applying the inverse
    // relation. Each iteration recovers another group of source bits.
    uint32_t unshift_left_xor_mask(uint32_t value, unsigned shift, uint32_t mask)
    {
        uint32_t result = value;
        for (unsigned amount = shift; amount < 32; amount += shift)
            result = value ^ ((result << shift) & mask);
        return result;
    }
}

uint32_t untemper(uint32_t value)
{
    // MT19937 tempering, in forward order:
    // y ^= (y >> 11);
    // y ^= (y << 7) & 0x9d2c5680;
    // y ^= (y << 15) & 0xefc60000;
    // y ^= (y >> 18);

    value = unshift_right_xor(value, 18);
    value = unshift_left_xor_mask(value, 15, 0xefc60000);
    value = unshift_left_xor_mask(value, 7, 0x9d2c5680);
    value = unshift_right_xor(value, 11);

    return value;
}

set03::ex21::MT19937 clone_mt19937(set03::ex21::MT19937 &original)
{
    constexpr size_t state_size = 624;
    std::vector<uint32_t> state;
    state.reserve(state_size);

    // One complete batch of outputs reveals every element of the state.
    for (size_t i = 0; i < state_size; ++i)
        state.push_back(untemper(original()));

    // The next call to either generator will twist the recovered state.
    set03::ex21::MT19937 clone(0);
    clone.set_state(state, state_size);

    return clone;
}

} // namespace set03::ex23
