#include "ex10.h"
#include <iostream>
#include <fstream>
#include "common.h"

int main(int argc, char** argv) {
  std::string filename = std::string(PROJECT_SOURCE_DIR) + "/resources/set02/ex10/10.txt";
  if (argc > 1) filename = argv[1];  
  std::ifstream ifs(filename);
  if (!ifs) {
      std::cerr << "Could not open file: " << filename << std::endl;
      return 2;
  }
  std::string b64, line;
  while (std::getline(ifs, line)) b64 += line;  
  auto cipher = base64_to_bytes(b64);
  std::string key = "YELLOW SUBMARINE";
  std::vector<unsigned char> iv(16, 0);  
  std::cout << set02::ex10::decrypt_aes_128_cbc(cipher, key, iv);
  return 0;
}
