#include "ex24.h"
#include "common.h"
#include <iostream>
#include <random>

int main()
{
    // Part 1: Break MT19937 stream cipher
    std::cout << "=== Part 1: Break MT19937 stream cipher ===" << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dist(0, 0xFFFF);
    uint16_t secret_seed = dist(gen);

    std::vector<unsigned char> known_plaintext(14, 'A');
    auto ciphertext = set03::ex24::encrypt_with_random_prefix(known_plaintext, secret_seed);

    std::cout << "Ciphertext length: " << ciphertext.size() << std::endl;
    std::cout << "Brute-forcing 16-bit seed..." << std::endl;

    uint16_t recovered = set03::ex24::recover_seed(ciphertext, known_plaintext);

    std::cout << "Secret seed:    " << secret_seed << std::endl;
    std::cout << "Recovered seed: " << recovered << std::endl;
    std::cout << (recovered == secret_seed ? "SUCCESS!" : "FAILURE") << std::endl;

    // Part 2: Password reset token check
    std::cout << "\n=== Part 2: Check password reset token ===" << std::endl;

    auto token = set03::ex24::generate_password_token();
    std::cout << "Token (hex): " << bytes_to_hex(token) << std::endl;

    bool is_time_seeded = set03::ex24::is_mt19937_time_token(token);
    std::cout << "Is MT19937 time-seeded token: " << (is_time_seeded ? "YES" : "NO") << std::endl;

    return (recovered == secret_seed && is_time_seeded) ? 0 : 1;
}
