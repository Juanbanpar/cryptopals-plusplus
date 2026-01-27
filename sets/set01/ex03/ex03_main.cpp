#include "ex03.h"
#include <iostream>

int main(int argc, char **argv) {
  std::string input;
  if (argc > 1) input = argv[1];
  else if (!std::getline(std::cin, input)) return 1;
  try {
    auto [key, plain] = set01::ex03::crack_single_byte_xor(input);
    std::cout << "key=0x" << std::hex << (int)key << std::dec << "\n" << plain << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 2;
  }
  return 0;
}
