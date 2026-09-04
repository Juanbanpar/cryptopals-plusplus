#include "common.h"
#include "ex28.h"
#include "ex29.h"
#include <iostream>

int main()
{
    const std::vector<unsigned char> key{'s', 'e', 'c', 'r', 'e', 't'};
    const std::string original_string =
        "comment1=cooking%20MCs;userdata=foo;comment2=%20like%20a%20pound%20of%20bacon";
    const std::string append_string = ";admin=true";
    const std::vector<unsigned char> original(original_string.begin(), original_string.end());
    const std::vector<unsigned char> append_data(append_string.begin(), append_string.end());
    const auto original_mac = set04::ex28::sha1_mac(key, original);
    const auto result = set04::ex29::forge_sha1_mac(
        original,
        original_mac,
        append_data,
        [&key](const auto &message, const auto &mac)
        { return set04::ex28::verify_sha1_mac(key, message, mac); },
        0,
        32
    );

    if (result.first.empty())
        return 1;
    std::cout << "Forged message: " << std::string(result.first.begin(), result.first.end()) << '\n'
              << "Forged MAC: " << bytes_to_hex(result.second) << std::endl;
    return 0;
}
