#include "ex08.h"
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "usage: ex08 <hex-file>\n";
    return 2;
  }
  auto line = set01::ex08::detect_aes_128_ecb_in_hex_file(argv[1]);
  std::cout << line << "\n";
  return 0;
}
