#include "ex36.h"
#include <iostream>

int main()
{
    const auto parameters = set05::ex36::toy_parameters();
    const std::vector<unsigned char> password{'c', 'o', 'r', 'r', 'e', 'c', 't'};
    set05::ex36::Server server(parameters, "alice@example.com", password, {'s', 'a', 'l', 't'}, 11);
    const set05::ex36::Client client(parameters, "alice@example.com", password, 7);
    const bool success = set05::ex36::authenticate(server, client);

    std::cout << "SRP authentication: " << (success ? "SUCCESS" : "FAILURE") << std::endl;
    return success ? 0 : 1;
}
