#include "ex14.h"
#include <iostream>

int main()
{
    auto oracle = set02::ex14::create_oracle();
    std::string secret = set02::ex14::break_ecb_harder(oracle);
    std::cout << secret << std::endl;
    return 0;
}
