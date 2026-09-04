#include "aes.h"
#include "common.h"
#include "ex27.h"
#include <iostream>

int main()
{
    const std::string message =
        "This message is long enough to occupy three CBC blocks.";
    const std::vector<unsigned char> plaintext(message.begin(), message.end());
    const auto ciphertext = set04::ex27::encrypt_message(plaintext);
    const auto recovered_key = set04::ex27::recover_key(ciphertext);
    const auto recovered = aes::decrypt_cbc(ciphertext, recovered_key, recovered_key);

    std::cout << "Recovered key: " << bytes_to_hex(recovered_key) << '\n'
              << "Recovered plaintext: "
              << std::string(recovered.begin(), recovered.end()) << std::endl;
    return recovered == plaintext ? 0 : 1;
}
