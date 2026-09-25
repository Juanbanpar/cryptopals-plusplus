#include <gtest/gtest.h>

#include "ex48.h"

TEST(Ex48, CompleteOracleUsesPkcs1Prefix)
{
    const auto keys = set05::ex39::generate_key_pair(64);
    const size_t block_size = (keys.public_key.n.bit_length() + 7) / 8;
    const auto ciphertext = set06::ex47::encrypt_padded(
        std::vector<unsigned char>{'x'}, keys.public_key, block_size
    );
    EXPECT_TRUE(set06::ex47::padding_oracle(
        ciphertext, keys.private_key, block_size
    ));
}
