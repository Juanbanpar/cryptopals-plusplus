#include "ex43.h"

#include <iostream>

int main()
{
    const auto recovery = set06::ex43::recover_key_from_small_nonce(
        set06::ex43::challenge_message(),
        set06::ex43::challenge_signature(),
        set06::ex43::challenge_public_key()
    );
    if (!recovery.found)
    {
        std::cerr << "No nonce found in the expected range" << std::endl;
        return 1;
    }
    std::cout << "Nonce: " << recovery.nonce.to_hex() << '\n'
              << "Private key: " << recovery.private_key.to_hex() << '\n'
              << "SHA-1 fingerprint: "
              << set06::dsa::private_key_fingerprint(recovery.private_key) << std::endl;
    return set06::dsa::private_key_fingerprint(recovery.private_key) ==
                   "0954edd5e0afe5542a4adf012611a91912a3ec16"
               ? 0
               : 1;
}
