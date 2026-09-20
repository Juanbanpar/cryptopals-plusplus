#include "ex36.h"
#include "ex37.h"
#include <iostream>

int main()
{
    const auto parameters = set05::ex36::toy_parameters();
    const std::vector<unsigned char> password{'s', 'e', 'c', 'r', 'e', 't'};
    set05::ex36::Server server(parameters, "user@example.com", password, {'s', 'a', 'l', 't'}, 11);

    bool success = true;
    for (const auto &public_key : {
             bignum::BigUint(0),
             parameters.n,
             parameters.n * bignum::BigUint(2)})
    {
        const auto result = set05::ex37::zero_key_attack(server, public_key);
        success = success && result.accepted;
        std::cout << "A = " << public_key.to_hex()
                  << ", authentication: " << (result.accepted ? "BYPASSED" : "REJECTED")
                  << '\n';
    }
    return success ? 0 : 1;
}
