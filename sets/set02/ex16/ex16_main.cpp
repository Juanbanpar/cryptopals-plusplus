#include "ex16.h"
#include "common.h"
#include <iostream>

int main()
{
    // Show that sanitized input does not grant admin
    std::string probe = ";admin=true;";
    auto clean_cipher = set02::ex16::encrypt_userdata(probe);

    std::cout << "Clean ciphertext (hex): " << bytes_to_hex(clean_cipher) << std::endl;
    std::cout << "Clean is_admin: " << (set02::ex16::is_admin(clean_cipher) ? "true" : "false") << std::endl << std::endl;

    // Perform the CBC bitflipping attack
    auto attack_cipher = set02::ex16::create_admin_ciphertext();
    std::cout << "Attack ciphertext (hex): " << bytes_to_hex(attack_cipher) << std::endl;

    bool success = set02::ex16::is_admin(attack_cipher);
    if (success)
    {
        std::cout << "Attack result: SUCCESS (role=admin)" << std::endl;
        return 0;
    }

    std::cout << "Attack result: FAILURE" << std::endl;
    return 1;
}
