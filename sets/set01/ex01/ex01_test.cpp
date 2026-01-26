#include "ex01.h"
#include <gtest/gtest.h>

TEST(HexToBase64, Example) {
  const std::string hex = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e";
  const std::string expected = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29u";
  EXPECT_EQ(hex_to_base64(hex), expected);
}

