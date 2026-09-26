#include <gtest/gtest.h>

#include "ex47.h"

TEST(Ex47, BuildsAndChecksPkcs1V15Blocks)
{
    const auto keys = set05::ex39::generate_key_pair(64);
    const size_t block_size = (keys.public_key.n.bit_length() + 7) / 8;
    const std::vector<unsigned char> message{'x'};
    const auto padded = set06::ex47::pad_message(message, block_size);
    const auto plaintext = bignum::BigUint::from_bytes_be(padded);
    const auto ciphertext = set05::ex39::encrypt(plaintext, keys.public_key);

    EXPECT_EQ(padded[0], 0);
    EXPECT_EQ(padded[1], 2);
    EXPECT_TRUE(set06::ex47::has_pkcs1_v15_prefix(plaintext, block_size));
    EXPECT_TRUE(set06::ex47::padding_oracle(ciphertext, keys.private_key, block_size));
}
