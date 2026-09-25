#include "ex44.h"

#include <fstream>
#include <iostream>

#ifndef EX44_RESOURCE
#define EX44_RESOURCE "resources/set06/ex44/44.txt"
#endif

int main()
{
    std::ifstream input(EX44_RESOURCE);
    if (!input)
    {
        std::cerr << "Unable to open challenge signature data" << std::endl;
        return 1;
    }
    const auto messages = set06::ex44::load_signatures(input);
    const auto recovery = set06::ex44::recover_reused_nonce(
        messages, set06::ex44::challenge_public_key()
    );
    if (!recovery.found)
    {
        std::cerr << "No repeated nonce found" << std::endl;
        return 1;
    }
    std::cout << "Repeated signatures: " << recovery.first_signature + 1 << " and "
              << recovery.second_signature + 1 << '\n'
              << "Nonce: " << recovery.nonce.to_hex() << '\n'
              << "Private key: " << recovery.private_key.to_hex() << '\n'
              << "SHA-1 fingerprint: "
              << set06::dsa::private_key_fingerprint(recovery.private_key) << std::endl;
    return set06::dsa::private_key_fingerprint(recovery.private_key) ==
                   "ca8f6f7c66fa362d40760d135b763eb8527d3d52"
               ? 0
               : 1;
}
