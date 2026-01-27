#include "ex05.h"
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "usage: ex05 <key> [text]\n";
    return 2;
  }
  std::string key = argv[1];
  std::string text;
  if (argc > 2) text = argv[2];
  else if (!std::getline(std::cin, text)) return 1;
  try {
    std::cout << set01::ex05::repeating_key_xor_hex_from_text(text, key) << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 3;
  }
  return 0;
}
