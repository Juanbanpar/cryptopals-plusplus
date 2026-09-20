#include "common.h"
#include "ex34.h"
#include <iostream>

int main()
{
    const std::string text = "DH parameter injection reaches both CBC messages";
    const std::vector<unsigned char> message(text.begin(), text.end());
    const auto result = set05::ex34::parameter_injection_attack(message);

    std::cout << "Recovered Alice message: "
              << std::string(result.decrypted_alice_message.begin(), result.decrypted_alice_message.end())
              << '\n'
              << "Recovered Bob message: "
              << std::string(result.decrypted_bob_message.begin(), result.decrypted_bob_message.end())
              << '\n'
              << "Verification: "
              << (result.decrypted_alice_message == message &&
                          result.decrypted_bob_message == message
                      ? "SUCCESS"
                      : "FAILURE")
              << std::endl;
    return result.decrypted_alice_message == message &&
                   result.decrypted_bob_message == message
               ? 0
               : 1;
}
