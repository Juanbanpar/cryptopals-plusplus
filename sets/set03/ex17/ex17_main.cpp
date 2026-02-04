#include "ex17.h"
#include "common.h"
#include <iostream>

int main()
{
    auto [ciphertext, iv] = set03::ex17::encrypt_random_string();
    std::cout << "Ciphertext (hex): " << bytes_to_hex(ciphertext) << std::endl;
    std::cout << "IV (hex): " << bytes_to_hex(iv) << std::endl << std::endl;

    auto recovered = set03::ex17::padding_oracle_attack(ciphertext, iv);
    std::string plaintext(recovered.begin(), recovered.end());

    std::cout << "Recovered plaintext: " << plaintext << std::endl;
    return 0;
}
