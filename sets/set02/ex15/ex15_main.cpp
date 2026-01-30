#include "ex15.h"
#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        std::string s = "ICE ICE BABY\x04\x04\x04\x04";
        std::cout << "Input: " << s << " (size " << s.size() << ")" << std::endl;
        std::string stripped = set02::ex15::validate_and_strip_padding_str(s);
        std::cout << "Stripped: " << stripped << std::endl;

        std::string bad = "ICE ICE BABY\x05\x05\x05\x05";
        std::cout << "Input bad: " << bad << std::endl;
        set02::ex15::validate_and_strip_padding_str(bad);
    }
    catch (const std::exception &e)
    {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    return 0;
}
