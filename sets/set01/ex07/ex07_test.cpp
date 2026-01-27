#include "ex07.h"
#include <gtest/gtest.h>

TEST(DecryptAES128ECB, OfficialFile) {
  const std::string path = std::string(PROJECT_SOURCE_DIR) + "/resources/set01/ex07/7.txt";
  auto plain = set01::ex07::decrypt_aes_128_ecb_base64_file(path, "YELLOW SUBMARINE");
  // verify expected start
  EXPECT_NE(plain.find("I'm back and I'm ringin' the bell"), std::string::npos);
}
