#include "ex21.h"

namespace set03::ex21
{

    MT19937::MT19937(uint32_t seed) : mt(N), mti(N + 1)
    {
        this->seed(seed);
    }

    void MT19937::seed(uint32_t s)
    {
        mt[0] = s;
        for (size_t i = 1; i < N; ++i)
        {
            mt[i] = 1812433253 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + (uint32_t)i;
        }
        mti = N;
    }

    void MT19937::twist()
    {
        for (size_t i = 0; i < N; ++i)
        {
            uint32_t y = (mt[i] & UPPER_MASK) | (mt[(i + 1) % N] & LOWER_MASK);
            mt[i] = mt[(i + M) % N] ^ (y >> 1);
            if (y & 1)
                mt[i] ^= MATRIX_A;
        }
        mti = 0;
    }

    uint32_t MT19937::operator()()
    {
        if (mti >= N)
            twist();

        uint32_t y = mt[mti++];

        // Tempering
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680;
        y ^= (y << 15) & 0xefc60000;
        y ^= (y >> 18);

        return y;
    }

    void MT19937::set_state(const std::vector<uint32_t> &state, size_t idx)
    {
        if (state.size() == N)
        {
            mt = state;
            mti = idx;
        }
    }

} // namespace set03::ex21
