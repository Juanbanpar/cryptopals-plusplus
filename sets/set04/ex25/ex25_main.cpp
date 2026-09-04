#include "ex25.h"
#include <iostream>

int main()
{
    const auto ciphertext = set04::ex25::encrypt_challenge_plaintext();
    const auto plaintext = set04::ex25::recover_plaintext(ciphertext);

    std::cout << "Recovered plaintext:\n"
              << std::string(plaintext.begin(), plaintext.end()) << std::endl;
    return 0;
}
