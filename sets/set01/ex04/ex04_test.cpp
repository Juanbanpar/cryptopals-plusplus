#include "ex04.h"
#include <gtest/gtest.h>

TEST(Ex04, DetectSingleCharXOR)
{
    // path built from project source dir (set by CMake)
    const std::string path = std::string(PROJECT_SOURCE_DIR) + "/resources/set01/ex04/4.txt";
    auto [plain, key, hexline] = set01::ex04::detect_single_char_xor(path);
    const std::string expected = "Now that the party is jumping";
    EXPECT_EQ(plain, expected);
}
