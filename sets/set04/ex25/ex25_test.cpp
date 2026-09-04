#include <gtest/gtest.h>
#include "aes.h"
#include "common.h"
#include "ex25.h"
#include <algorithm>
#include <string>

TEST(Ex25, EditCiphertext)
{
    const std::string key_string = "YELLOW SUBMARINE";
    const std::vector<unsigned char> key(key_string.begin(), key_string.end());
    const std::string original = "CTR editing keeps the ciphertext length";
    const std::vector<unsigned char> plaintext(original.begin(), original.end());
    const auto ciphertext = aes::ctr(plaintext, key, 0);

    const std::vector<unsigned char> replacement{'X', 'Y', 'Z'};
    const auto edited = set04::ex25::edit_ciphertext(ciphertext, key, 4, replacement);
    const auto decrypted = aes::ctr(edited, key, 0);

    auto expected = plaintext;
    std::copy(replacement.begin(), replacement.end(), expected.begin() + 4);
    EXPECT_EQ(decrypted, expected);
    EXPECT_THROW(
        set04::ex25::edit_ciphertext(ciphertext, key, ciphertext.size() + 1, {}),
        std::invalid_argument
    );
}

TEST(Ex25, RecoverPlaintextThroughEditOracle)
{
    const auto ciphertext = set04::ex25::encrypt_challenge_plaintext();
    const auto plaintext = set04::ex25::recover_plaintext(ciphertext);
    const std::string recovered(plaintext.begin(), plaintext.end());

    EXPECT_NE(recovered.find("Play that funky music"), std::string::npos);
}
