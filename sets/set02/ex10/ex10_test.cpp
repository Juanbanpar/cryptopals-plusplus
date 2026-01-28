#include <gtest/gtest.h>
#include <fstream>
#include "aes.h"
#include "common.h"
#include "ex10.h"

TEST(Ex10, Solution) {
  // The solution should contain the lyrics "Play that funky music"
  // We check substring to verify correctness without asserting exact content length/newlines
  std::string filename = std::string(PROJECT_SOURCE_DIR) + "/resources/set02/ex10/10.txt";
  std::ifstream ifs(filename);
  ASSERT_TRUE(ifs.good());
  std::string b64, line;
  while(std::getline(ifs, line)) b64 += line;
  auto cipher = base64_to_bytes(b64);
  std::string key = "YELLOW SUBMARINE";
  std::vector<uint8_t> iv(16, 0);  
  std::string res = set02::ex10::decrypt_aes_128_cbc(cipher, key, iv);
  EXPECT_TRUE(res.find("Play that funky music") != std::string::npos);
}
