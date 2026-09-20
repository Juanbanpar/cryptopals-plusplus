#include "ex33.h"
#include "ex35.h"
#include <iostream>

int main()
{
    const auto parameters = set05::ex33::toy_parameters();
    const std::string text = "negotiated groups are attacker controlled";
    const std::vector<unsigned char> message(text.begin(), text.end());
    bool success = true;
    for (const auto &generator : {
             bignum::BigUint(1),
             parameters.p,
             parameters.p - bignum::BigUint(1)})
    {
        const auto result = set05::ex35::malicious_generator_attack(
            message, parameters, generator
        );
        success = success && result.decrypted_alice_message == message &&
                  result.decrypted_bob_message == message;
        std::cout << "g = " << generator.to_hex()
                  << ", recovered secret = " << result.predictable_secret.to_hex() << '\n';
    }
    std::cout << "Verification: " << (success ? "SUCCESS" : "FAILURE") << std::endl;
    return success ? 0 : 1;
}
