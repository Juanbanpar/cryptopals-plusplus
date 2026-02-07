#include "ex08.h"
#include <gtest/gtest.h>

TEST(Ex08, DetectAES128ECB)
{
    const std::string path = std::string(PROJECT_SOURCE_DIR) + "/resources/set01/ex08/8.txt";
    auto line = set01::ex08::detect_aes_128_ecb_in_hex_file(path);
    // known ciphertext that contains repeated 16-byte blocks
    const std::string expected_start = "d880619740a8a19b7840a8a31c810a3d";
    EXPECT_NE(line.find(expected_start), std::string::npos);
}
