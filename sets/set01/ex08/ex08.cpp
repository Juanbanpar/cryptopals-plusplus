#include "ex08.h"
#include "../../lib/common/common.h"

#include <fstream>
#include <unordered_set>
#include <sstream>

std::string set01::ex08::detect_aes_128_ecb_in_hex_file(const std::string &path) {
  std::ifstream ifs(path);
  if (!ifs) return std::string();
  std::string line;
  size_t best_dup = 0;
  std::string best_line;
  while (std::getline(ifs, line)) {
    if (line.empty()) continue;
    // trim
    std::string hex = line;
    // convert hex to bytes
    auto bytes = hex_to_bytes(hex);
    // split into 16-byte blocks
    std::unordered_set<std::string> seen;
    size_t duplicates = 0;
    for (size_t i = 0; i + 16 <= bytes.size(); i += 16) {
      std::string block(reinterpret_cast<char*>(const_cast<unsigned char*>(&bytes[i])), 16);
      if (seen.find(block) != seen.end()) duplicates++;
      else seen.insert(block);
    }
    if (duplicates > best_dup) {
      best_dup = duplicates;
      best_line = line;
    }
  }
  return best_line;
}
