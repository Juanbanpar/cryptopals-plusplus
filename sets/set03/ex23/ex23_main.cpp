#include "ex23.h"
#include <iostream>

int main()
{
    // Create original RNG with random seed
    set03::ex21::MT19937 original(12345);

    // Skip some outputs
    for (int i = 0; i < 100; ++i)
        original();

    std::cout << "Cloning MT19937 by observing 624 outputs..." << std::endl;

    auto clone = set03::ex23::clone_mt19937(original);

    std::cout << "Verifying clone produces same outputs:" << std::endl;
    bool success = true;
    for (int i = 0; i < 10; ++i)
    {
        uint32_t orig_val = original();
        uint32_t clone_val = clone();
        std::cout << "Original: " << orig_val << " | Clone: " << clone_val;
        if (orig_val == clone_val)
            std::cout << " ✓" << std::endl;
        else
        {
            std::cout << " ✗" << std::endl;
            success = false;
        }
    }

    std::cout << (success ? "SUCCESS: Clone matches original!" : "FAILURE: Clone mismatch") << std::endl;
    return success ? 0 : 1;
}
