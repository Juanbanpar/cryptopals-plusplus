#include <gtest/gtest.h>
#include "ex24.h"
#include <random>

TEST(Ex24, MT19937Cipher)
{
    std::string plaintext = "Hello, MT19937 cipher!";
    std::vector<unsigned char> pt(plaintext.begin(), plaintext.end());
    uint16_t seed = 12345;

    auto ciphertext = set03::ex24::mt19937_cipher(pt, seed);
    auto decrypted = set03::ex24::mt19937_cipher(ciphertext, seed);

    EXPECT_EQ(pt, decrypted);
}

TEST(Ex24, RecoverSeed)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dist(0, 0xFFFF);
    uint16_t secret_seed = dist(gen);

    std::vector<unsigned char> known_plaintext(14, 'A');
    auto ciphertext = set03::ex24::encrypt_with_random_prefix(known_plaintext, secret_seed);

    uint16_t recovered = set03::ex24::recover_seed(ciphertext, known_plaintext);
    EXPECT_EQ(recovered, secret_seed);
}

TEST(Ex24, PasswordTokenCheck)
{
    auto token = set03::ex24::generate_password_token();
    EXPECT_TRUE(set03::ex24::is_mt19937_time_token(token, 5));
}
