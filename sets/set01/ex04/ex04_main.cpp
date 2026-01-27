#include "ex04.h"
#include <iostream>

int main(int argc, char **argv) {
  std::string default_path = std::string(PROJECT_SOURCE_DIR) + "/resources/set01/ex04/4.txt";
  std::string path = (argc > 1) ? argv[1] : default_path;
  try {
    auto [plain, key, hexline] = set01::ex04::detect_single_char_xor(path);
    std::cout << "key=0x" << std::hex << (int)key << std::dec << "\n" << plain << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "error: " << e.what() << std::endl;
    return 2;
  }
  return 0;
}
