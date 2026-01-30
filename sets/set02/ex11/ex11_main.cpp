#include "ex11.h"
#include <iostream>

int main()
{
    // Run the detection a few times
    for (int i = 0; i < 10; ++i)
    {
        std::string actual_mode;
        auto oracle = [&](const std::vector<unsigned char> &input)
        {
            return set02::ex11::encryption_oracle(input, &actual_mode);
        };
        std::string detected = set02::ex11::detect_mode(oracle);
        std::cout << "Actual: " << actual_mode << ", Detected: " << detected << std::endl;
    }
    return 0;
}
