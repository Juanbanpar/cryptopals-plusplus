#include <gtest/gtest.h>
#include "common.h"
#include "ex36.h"
#include "sha256.h"

TEST(Ex36, SHA256AndHMACKnownVectors)
{
    const std::vector<unsigned char> empty;
    const std::vector<unsigned char> abc{'a', 'b', 'c'};
    EXPECT_EQ(
        bytes_to_hex(sha256::digest(empty)),
        "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"
    );
    EXPECT_EQ(
        bytes_to_hex(sha256::digest(abc)),
        "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"
    );

    const std::vector<unsigned char> key(20, 0x0b);
    const std::vector<unsigned char> message{'H', 'i', ' ', 'T', 'h', 'e', 'r', 'e'};
    EXPECT_EQ(
        bytes_to_hex(set05::ex36::hmac_sha256(key, message)),
        "b0344c61d8db38535ca8afceaf0bf12b881dc200c9833da726e9376c2e32cff7"
    );
}

TEST(Ex36, SRPAuthenticatesOnlyWithTheCorrectPassword)
{
    const auto parameters = set05::ex36::toy_parameters();
    const std::vector<unsigned char> password{'p', 'a', 's', 's', 'w', 'o', 'r', 'd'};
    const std::vector<unsigned char> salt{'f', 'i', 'x', 'e', 'd'};
    set05::ex36::Server server(parameters, "user@example.com", password, salt, 11);
    const set05::ex36::Client correct(parameters, "user@example.com", password, 7);
    const set05::ex36::Client wrong(
        parameters,
        "user@example.com",
        std::vector<unsigned char>{'w', 'r', 'o', 'n', 'g'},
        7
    );

    EXPECT_TRUE(set05::ex36::authenticate(server, correct));
    EXPECT_FALSE(set05::ex36::authenticate(server, wrong));
}

TEST(Ex36, SRPWorksWithTheNistGroup)
{
    const auto parameters = set05::ex36::default_parameters();
    const std::vector<unsigned char> password{'n', 'i', 's', 't'};
    set05::ex36::Server server(
        parameters,
        "user@example.com",
        password,
        std::vector<unsigned char>(16, 0x42),
        11
    );
    const set05::ex36::Client client(parameters, "user@example.com", password, 7);

    EXPECT_TRUE(set05::ex36::authenticate(server, client));
}
