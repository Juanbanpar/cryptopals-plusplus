#include "ex09.h"
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "usage: ex09 <block-size> <text>\n";
    return 2;
  }
  size_t block = std::stoul(argv[1]);
  std::string text = argv[2];
  auto out = set02::ex09::pkcs7_pad_string(text, block);
  // print raw bytes
  std::cout << out;
  return 0;
}
