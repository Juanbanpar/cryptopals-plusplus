#include <gtest/gtest.h>
#include "aes.h"
#include "ex27.h"
#include <algorithm>

TEST(Ex27, RecoverKeyFromCBCWithKeyAsIV)
{
    const std::string message =
        "This message is long enough to occupy three CBC blocks.";
    const std::vector<unsigned char> plaintext(message.begin(), message.end());
    const auto ciphertext = set04::ex27::encrypt_message(plaintext);
    const auto recovered_key = set04::ex27::recover_key(ciphertext);

    EXPECT_EQ(aes::decrypt_cbc(ciphertext, recovered_key, recovered_key), plaintext);
}

TEST(Ex27, InvalidAsciiErrorCarriesPlaintext)
{
    const std::vector<unsigned char> plaintext(48, 'A');
    const auto ciphertext = set04::ex27::encrypt_message(plaintext);
    std::vector<unsigned char> modified(ciphertext.begin(), ciphertext.begin() + 48);
    std::copy(ciphertext.begin(), ciphertext.begin() + 16, modified.begin());
    std::fill(modified.begin() + 16, modified.begin() + 32, 0);
    std::copy(ciphertext.begin(), ciphertext.begin() + 16, modified.begin() + 32);

    try
    {
        set04::ex27::decrypt_message(modified);
        FAIL() << "expected an invalid ASCII error";
    }
    catch (const set04::ex27::InvalidAsciiError &error)
    {
        EXPECT_EQ(error.plaintext().size(), 48);
    }
}
