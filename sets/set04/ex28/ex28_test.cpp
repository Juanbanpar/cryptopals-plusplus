#include <gtest/gtest.h>
#include "common.h"
#include "ex28.h"
#include "sha1.h"
#include <string>

TEST(Ex28, SHA1KnownVectors)
{
    const std::vector<unsigned char> empty;
    const std::vector<unsigned char> abc{'a', 'b', 'c'};
    const std::string long_message =
        "abcdbcdecdefdefgefghfghighijhijkijkljklmklmn"
        "lmnomnopnopq";
    const std::vector<unsigned char> long_data(long_message.begin(), long_message.end());

    EXPECT_EQ(bytes_to_hex(sha1::digest(empty)),
              "da39a3ee5e6b4b0d3255bfef95601890afd80709");
    EXPECT_EQ(bytes_to_hex(sha1::digest(abc)),
              "a9993e364706816aba3e25717850c26c9cd0d89d");
    EXPECT_EQ(bytes_to_hex(sha1::digest(long_data)),
              "84983e441c3bd26ebaae4aa1f95129e5e54670f1");
}

TEST(Ex28, SecretPrefixMAC)
{
    const std::vector<unsigned char> key{'k', 'e', 'y'};
    const std::vector<unsigned char> message{'m', 'e', 's', 's', 'a', 'g', 'e'};
    const auto mac = set04::ex28::sha1_mac(key, message);

    EXPECT_TRUE(set04::ex28::verify_sha1_mac(key, message, mac));
    auto tampered = message;
    tampered[0] ^= 1;
    EXPECT_FALSE(set04::ex28::verify_sha1_mac(key, tampered, mac));
    EXPECT_FALSE(set04::ex28::verify_sha1_mac(key, message, {}));
}
