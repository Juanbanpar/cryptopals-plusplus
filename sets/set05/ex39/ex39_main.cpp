#include "ex39.h"
#include <iostream>

int main()
{
    const auto key_pair = set05::ex39::generate_key_pair();
    const std::string text = "textbook RSA";
    const std::vector<unsigned char> message(text.begin(), text.end());
    const auto ciphertext = set05::ex39::encrypt_message(message, key_pair.public_key);
    const auto plaintext = set05::ex39::decrypt_message(
        ciphertext, key_pair.private_key, message.size()
    );

    std::cout << "Ciphertext: " << ciphertext.to_hex() << '\n'
              << "Recovered message: "
              << std::string(plaintext.begin(), plaintext.end()) << '\n'
              << "Verification: " << (plaintext == message ? "SUCCESS" : "FAILURE")
              << std::endl;
    return plaintext == message ? 0 : 1;
}
