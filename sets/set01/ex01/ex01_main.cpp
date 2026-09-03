#include "ex01.h"
#include <iostream>

int main(int argc, char **argv)
{
    std::string input;
    if (argc > 1)
    {
        input = argv[1];
    }
    else
    {
        if (!std::getline(std::cin, input))
            return 1;
    }
    try
    {
        std::cout << hex_to_base64(input) << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        return 2;
    }
    return 0;
}
