#include "ex46.h"

#include "base64.h"

#include <iostream>

int main()
{
    const auto keys = set05::ex39::generate_key_pair(512);
    const auto message = base64_to_bytes(
        "VGhhdCdzIHdoeSBJIGZvdW5kIHlvdSBkb24ndCBwbGF5IGFyb3VuZCB3aXRoIHRoZSBGdW5reSBDb2xkIE1lZGluYQ=="
    );
    const auto ciphertext = set05::ex39::encrypt_message(message, keys.public_key);
    const auto parity_oracle = [&keys](const bignum::BigUint &candidate) {
        return set05::ex39::decrypt(candidate, keys.private_key).is_odd();
    };
    const auto recovered = set06::ex46::recover_plaintext(
        ciphertext, keys.public_key, parity_oracle
    );
    const auto plaintext = set05::ex39::integer_to_bytes(recovered);
    std::cout << std::string(plaintext.begin(), plaintext.end()) << std::endl;
    return plaintext == message ? 0 : 1;
}
