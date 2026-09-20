#include "ex33.h"
#include <iostream>

int main()
{
    const auto parameters = set05::ex33::toy_parameters();
    const auto alice = set05::ex33::generate_key_pair(parameters);
    const auto bob = set05::ex33::generate_key_pair(parameters);
    const auto alice_secret = set05::ex33::derive_shared_secret(
        parameters, alice.private_key, bob.public_key
    );
    const auto bob_secret = set05::ex33::derive_shared_secret(
        parameters, bob.private_key, alice.public_key
    );

    std::cout << "Alice session key: " << alice_secret.to_hex() << '\n'
              << "Bob session key: " << bob_secret.to_hex() << '\n'
              << "Verification: " << (alice_secret == bob_secret ? "SUCCESS" : "FAILURE")
              << std::endl;
    return alice_secret == bob_secret ? 0 : 1;
}
