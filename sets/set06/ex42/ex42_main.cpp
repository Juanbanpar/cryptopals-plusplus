#include "ex42.h"

#include <iostream>

int main()
{
    const auto keys = set05::ex39::generate_key_pair(512);
    const std::string text = "hi mom";
    const std::vector<unsigned char> message(text.begin(), text.end());
    const auto signature = set06::ex42::forge_signature(
        message, keys.public_key, 128
    );
    const bool valid = set06::ex42::verify_signature_weakly(
        message, signature, keys.public_key, 128
    );
    std::cout << "Forged signature: " << signature.to_hex() << '\n'
              << "Verification: " << (valid ? "SUCCESS" : "FAILURE") << std::endl;
    return valid ? 0 : 1;
}
