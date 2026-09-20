#include <gtest/gtest.h>
#include "ex38.h"

TEST(Ex38, OfflineDictionaryAttackRecoversPassword)
{
    const auto parameters = set05::ex36::toy_parameters();
    const std::vector<unsigned char> password{'s', 'e', 'c', 'r', 'e', 't'};
    const set05::ex38::Client client(parameters, "user@example.com", password, 7);
    const auto attacker_challenge = set05::ex38::make_attacker_challenge(
        parameters,
        {'s', 'a', 'l', 't'},
        bignum::BigUint(11),
        bignum::BigUint(19)
    );
    const auto hello = client.hello();
    const auto captured_hmac = client.response(attacker_challenge.challenge);
    const auto result = set05::ex38::offline_dictionary_attack(
        parameters,
        hello,
        attacker_challenge,
        captured_hmac,
        {{'p', 'a', 's', 's'}, password, {'s', 'e', 'c', 'r', 'e', 't', '2'}}
    );

    EXPECT_TRUE(result.found);
    EXPECT_EQ(result.password, password);
    EXPECT_EQ(result.attempts, 2U);
}

TEST(Ex38, RejectsDictionaryWithoutPassword)
{
    const auto parameters = set05::ex36::toy_parameters();
    const set05::ex38::Client client(
        parameters,
        "user@example.com",
        std::vector<unsigned char>{'s', 'e', 'c', 'r', 'e', 't'},
        7
    );
    const auto attacker_challenge = set05::ex38::make_attacker_challenge(
        parameters,
        {'s', 'a', 'l', 't'},
        bignum::BigUint(11),
        bignum::BigUint(19)
    );
    const auto result = set05::ex38::offline_dictionary_attack(
        parameters,
        client.hello(),
        attacker_challenge,
        client.response(attacker_challenge.challenge),
        {{'p', 'a', 's', 's'}}
    );
    EXPECT_FALSE(result.found);
    EXPECT_EQ(result.attempts, 1U);
}
