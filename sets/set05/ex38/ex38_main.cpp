#include "ex38.h"
#include <iostream>

int main()
{
    const auto parameters = set05::ex36::toy_parameters();
    const std::vector<unsigned char> password{'s', 'e', 'c', 'r', 'e', 't'};
    const set05::ex38::Client client(parameters, "user@example.com", password, 7);
    const auto attacker_challenge = set05::ex38::make_attacker_challenge(
        parameters,
        {'s', 'a', 'l', 't'},
        bignum::BigUint(11),
        bignum::BigUint(19)
    );
    const auto hello = client.hello();
    const auto captured_hmac = client.response(attacker_challenge.challenge);
    const auto result = set05::ex38::offline_dictionary_attack(
        parameters,
        hello,
        attacker_challenge,
        captured_hmac,
        {{'p', 'a', 's', 's'}, password, {'s', 'e', 'c', 'r', 'e', 't', '2'}}
    );

    std::cout << "Dictionary attack: " << (result.found ? "SUCCESS" : "FAILURE")
              << ", attempts: " << result.attempts << std::endl;
    return result.found ? 0 : 1;
}
