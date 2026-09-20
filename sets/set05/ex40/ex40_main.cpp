#include "ex40.h"
#include <iostream>

int main()
{
    const std::array<set05::ex39::KeyPair, 3> recipients = {
        set05::ex39::generate_key_pair(),
        set05::ex39::generate_key_pair(),
        set05::ex39::generate_key_pair()};
    const std::string text = "broadcast RSA";
    const std::vector<unsigned char> message(text.begin(), text.end());
    const auto ciphertexts = set05::ex40::encrypt_for_recipients(message, recipients);
    const auto recovered = set05::ex40::recover_message_bytes(ciphertexts, message.size());

    std::cout << "Recovered message: "
              << std::string(recovered.begin(), recovered.end()) << '\n'
              << "Verification: " << (recovered == message ? "SUCCESS" : "FAILURE")
              << std::endl;
    return recovered == message ? 0 : 1;
}
