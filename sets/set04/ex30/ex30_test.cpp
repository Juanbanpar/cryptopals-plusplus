#include <gtest/gtest.h>
#include "common.h"
#include "ex30.h"
#include "md4.h"
#include <string>

TEST(Ex30, MD4KnownVectors)
{
    const std::vector<unsigned char> empty;
    const std::vector<unsigned char> abc{'a', 'b', 'c'};
    const std::string message = "message digest";
    const std::vector<unsigned char> message_data(message.begin(), message.end());

    EXPECT_EQ(bytes_to_hex(md4::digest(empty)),
              "31d6cfe0d16ae931b73c59d7e0c089c0");
    EXPECT_EQ(bytes_to_hex(md4::digest(abc)),
              "a448017aaf21d8525fc10ae87aa6729d");
    EXPECT_EQ(bytes_to_hex(md4::digest(message_data)),
              "d9130a8164549fe818874806e1c7014b");
}

TEST(Ex30, SecretPrefixMACAndLengthExtension)
{
    const std::vector<unsigned char> key{'s', 'e', 'c', 'r', 'e', 't'};
    const std::string original_string =
        "comment1=cooking%20MCs;userdata=foo;comment2=%20like%20a%20pound%20of%20bacon";
    const std::string append_string = ";admin=true";
    const std::vector<unsigned char> original(original_string.begin(), original_string.end());
    const std::vector<unsigned char> append_data(append_string.begin(), append_string.end());
    const auto original_mac = set04::ex30::md4_mac(key, original);

    const auto result = set04::ex30::forge_md4_mac(
        original,
        original_mac,
        append_data,
        [&key](const auto &message, const auto &mac)
        { return set04::ex30::verify_md4_mac(key, message, mac); },
        0,
        32
    );

    ASSERT_FALSE(result.first.empty());
    EXPECT_TRUE(set04::ex30::verify_md4_mac(key, result.first, result.second));
    EXPECT_EQ(
        std::vector<unsigned char>(result.first.end() - append_data.size(), result.first.end()),
        append_data
    );
}
