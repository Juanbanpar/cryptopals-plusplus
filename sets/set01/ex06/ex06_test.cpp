#include "ex06.h"
#include "../../lib/common/common.h"
#include <gtest/gtest.h>

TEST(BreakRepeatingKeyXOR, OfficialResource) {
  const std::string path = std::string(PROJECT_SOURCE_DIR) + "/resources/set01/ex06/6.txt";
  auto [key, plain] = set01::ex06::break_repeating_key_xor_from_base64_file(path);
  EXPECT_EQ(key, "Terminator X: Bring the noise");
  EXPECT_NE(plain.find("I'm back"), std::string::npos);
}
