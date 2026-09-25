#include "ex48.h"

#include <algorithm>
#include <iostream>

int main()
{
    const auto keys = set05::ex39::generate_key_pair(384);
    const size_t block_size = (keys.public_key.n.bit_length() + 7) / 8;
    const std::string text = "kick it, CC";
    const std::vector<unsigned char> message(text.begin(), text.end());
    const auto ciphertext = set06::ex47::encrypt_padded(
        message, keys.public_key, block_size
    );
    const auto oracle = [&keys, block_size](const bignum::BigUint &candidate) {
        return set06::ex47::padding_oracle(candidate, keys.private_key, block_size);
    };
    const auto recovered = set06::ex48::recover_plaintext_complete(
        ciphertext, keys.public_key, oracle
    );
    const auto encoded = set05::ex39::integer_to_bytes(recovered, block_size);
    const auto delimiter = std::find(encoded.begin() + 2, encoded.end(), 0);
    if (delimiter == encoded.end())
        return 1;
    const std::vector<unsigned char> plaintext(delimiter + 1, encoded.end());
    std::cout << std::string(plaintext.begin(), plaintext.end()) << std::endl;
    return plaintext == message ? 0 : 1;
}
