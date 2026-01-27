#include "ex07.h"
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "usage: ex07 <base64-file>\n";
    return 2;
  }
  auto plain = set01::ex07::decrypt_aes_128_ecb_base64_file(argv[1]);
  std::cout << plain;
  return 0;
}
