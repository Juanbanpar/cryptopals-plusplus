#include "ex06.h"
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "usage: ex06 <base64-file>\n";
    return 2;
  }
  auto [key, plain] = set01::ex06::break_repeating_key_xor_from_base64_file(argv[1]);
  std::cout << "Key: " << key << "\n";
  std::cout << plain << "\n";
  return 0;
}
