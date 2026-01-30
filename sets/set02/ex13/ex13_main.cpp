#include "ex13.h"
#include "common.h"
#include <iostream>

int main()
{
    // Show a normal profile for an email
    std::string email = "foo@bar.com";
    auto normal_cipher = set02::ex13::encrypt_profile_for(email);
    auto normal_profile = set02::ex13::decrypt_profile(normal_cipher);

    std::cout << "Normal profile string: " << std::endl;
    std::cout << "  " << "email=" << normal_profile.email << "&uid=" << normal_profile.uid << "&role=" << normal_profile.role << std::endl;
    std::cout << "Encrypted (hex): " << bytes_to_hex(normal_cipher) << std::endl << std::endl;

    // Perform the cut-and-paste attack and show results
    auto attack_cipher = set02::ex13::create_admin_profile();
    auto attacked_profile = set02::ex13::decrypt_profile(attack_cipher);

    std::cout << "After attack, decrypted profile: " << std::endl;
    std::cout << "  " << "email=" << attacked_profile.email << "&uid=" << attacked_profile.uid << "&role=" << attacked_profile.role << std::endl;
    std::cout << "Attack ciphertext (hex): " << bytes_to_hex(attack_cipher) << std::endl;

    if (attacked_profile.role == "admin") {
        std::cout << "Attack result: SUCCESS (role=admin)" << std::endl;
        return 0;
    }

    std::cout << "Attack result: FAILURE" << std::endl;
    return 1;
}
