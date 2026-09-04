#include "common.h"
#include "ex28.h"
#include <iostream>

int main(int argc, char **argv)
{
    const std::string key_string = argc > 1 ? argv[1] : "secret";
    const std::string message_string = argc > 2
                                           ? argv[2]
                                           : "comment1=cooking%20MCs;userdata=foo";
    const std::vector<unsigned char> key(key_string.begin(), key_string.end());
    const std::vector<unsigned char> message(message_string.begin(), message_string.end());
    const auto mac = set04::ex28::sha1_mac(key, message);

    std::cout << bytes_to_hex(mac) << std::endl;
    return 0;
}
