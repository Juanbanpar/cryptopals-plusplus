#include <gtest/gtest.h>
#include "ex18.h"
#include "aes.h"

TEST(DecryptChallenge, Example)
{
    std::string result = set03::ex18::solve_challenge();
    EXPECT_TRUE(result.find("Yo, VIP Let's kick it Ice, Ice, baby Ice, Ice, baby ") != std::string::npos);
}

TEST(CTREncryptDecrypt, Example)
{
    std::string plaintext = "El CTR tumbando aguja por toda baraka";
    const std::string key_str = "JONAN DE BARAKA ";
    std::vector<uint8_t> key(key_str.begin(), key_str.end());
    uint64_t nonce = 12345;

    std::vector<uint8_t> pt(plaintext.begin(), plaintext.end());
    auto ciphertext = aes::ctr(pt, key, nonce);
    auto decrypted = aes::ctr(ciphertext, key, nonce);

    std::string result(decrypted.begin(), decrypted.end());
    EXPECT_EQ(result, plaintext);
}
