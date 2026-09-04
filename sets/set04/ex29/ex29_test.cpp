#include <gtest/gtest.h>
#include "ex28.h"
#include "ex29.h"

TEST(Ex29, ForgeSHA1LengthExtension)
{
    const std::vector<unsigned char> key{'s', 'e', 'c', 'r', 'e', 't'};
    const std::string original_string =
        "comment1=cooking%20MCs;userdata=foo;comment2=%20like%20a%20pound%20of%20bacon";
    const std::string append_string = ";admin=true";
    const std::vector<unsigned char> original(original_string.begin(), original_string.end());
    const std::vector<unsigned char> append_data(append_string.begin(), append_string.end());
    const auto original_mac = set04::ex28::sha1_mac(key, original);

    const auto result = set04::ex29::forge_sha1_mac(
        original,
        original_mac,
        append_data,
        [&key](const auto &message, const auto &mac)
        { return set04::ex28::verify_sha1_mac(key, message, mac); },
        0,
        32
    );

    ASSERT_FALSE(result.first.empty());
    EXPECT_TRUE(set04::ex28::verify_sha1_mac(key, result.first, result.second));
    EXPECT_EQ(
        std::vector<unsigned char>(result.first.end() - append_data.size(), result.first.end()),
        append_data
    );
}

TEST(Ex29, RejectsInvalidDigestAndRange)
{
    EXPECT_THROW(
        set04::ex29::forge_sha1_mac({}, std::vector<unsigned char>(19), {}, {}, 0, 1),
        std::invalid_argument
    );
    EXPECT_THROW(
        set04::ex29::forge_sha1_mac({}, std::vector<unsigned char>(20), {}, {}, 2, 1),
        std::invalid_argument
    );
}
