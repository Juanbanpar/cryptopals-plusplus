#include "ex26.h"
#include <iostream>

int main()
{
    const auto clean = set04::ex26::encrypt_userdata(";admin=true;");
    const auto attack = set04::ex26::create_admin_ciphertext();

    std::cout << "Clean is_admin: " << (set04::ex26::is_admin(clean) ? "true" : "false") << '\n';
    std::cout << "Attack result: "
              << (set04::ex26::is_admin(attack) ? "SUCCESS" : "FAILURE") << std::endl;
    return set04::ex26::is_admin(attack) ? 0 : 1;
}
