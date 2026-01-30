#include "ex12.h"
#include <iostream>

int main() {
    auto oracle = set02::ex12::create_oracle();
    std::string secret = set02::ex12::break_ecb_simple(oracle);
    std::cout << secret << std::endl;
    return 0;
}
