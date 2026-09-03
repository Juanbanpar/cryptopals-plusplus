#include "ex02.h"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "usage: ex02 <hex1> <hex2>\n";
        return 2;
    }
    try
    {
        std::cout << fixed_xor_hex(argv[1], argv[2]) << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        return 3;
    }
    return 0;
}
