#include "ex41.h"

#include <iostream>

int main()
{
    const auto keys = set05::ex39::generate_key_pair();
    const std::vector<unsigned char> message{'s', 'e', 'c', 'r', 'e', 't'};
    const auto ciphertext = set05::ex39::encrypt_message(message, keys.public_key);
    set06::ex41::DecryptionOracle oracle(keys.private_key);
    const auto recovered = set06::ex41::recover_plaintext(
        ciphertext,
        keys.public_key,
        oracle,
        bignum::BigUint(2)
    );
    const auto plaintext = set05::ex39::integer_to_bytes(recovered, message.size());
    std::cout << std::string(plaintext.begin(), plaintext.end()) << std::endl;
    return plaintext == message ? 0 : 1;
}
