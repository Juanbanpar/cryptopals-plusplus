#include <gtest/gtest.h>
#include "ex17.h"
#include "common.h"

TEST(PaddingOracleAttack, Example)
{
    // Run the attack multiple times to test different random strings
    for (int i = 0; i < 5; ++i)
    {
        auto [ciphertext, iv] = set03::ex17::encrypt_random_string();
        auto recovered = set03::ex17::padding_oracle_attack(ciphertext, iv);

        std::string plaintext(recovered.begin(), recovered.end());
        // All strings start with "000000", "000001", etc.
        EXPECT_TRUE(plaintext.find("00000") == 0) << "Recovered: " << plaintext;
    }
}
