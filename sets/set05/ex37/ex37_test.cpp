#include <gtest/gtest.h>
#include "ex37.h"

TEST(Ex37, ZeroModuloNBypassesSRP)
{
    const auto parameters = set05::ex36::toy_parameters();
    const std::vector<unsigned char> password{'s', 'e', 'c', 'r', 'e', 't'};
    set05::ex36::Server server(parameters, "user@example.com", password, {'s', 'a', 'l', 't'}, 11);

    for (const auto &public_key : {
             bignum::BigUint(0),
             parameters.n,
             parameters.n * bignum::BigUint(2)})
    {
        EXPECT_TRUE(set05::ex37::zero_key_attack(server, public_key).accepted);
    }
}
