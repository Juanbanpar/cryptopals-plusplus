#include "pkcs7.h"
#include <stdexcept>

namespace pkcs7 {

std::vector<unsigned char> pad(const std::vector<unsigned char>& data, size_t block_size) {
  if (block_size == 0) throw std::invalid_argument("block_size must be > 0");
  size_t pad_len = block_size - (data.size() % block_size);
  if (pad_len == 0) pad_len = block_size;
  std::vector<unsigned char> out = data;
  out.insert(out.end(), pad_len, static_cast<unsigned char>(pad_len));
  return out;
}

std::string pad_string(const std::string& s, size_t block_size) {
  std::vector<unsigned char> data(s.begin(), s.end());
  auto p = pad(data, block_size);
  return std::string(p.begin(), p.end());
}

std::vector<unsigned char> unpad(const std::vector<unsigned char>& data) {
  if (data.empty()) return {};
  unsigned char pad = data.back();
  if (pad == 0 || pad > data.size()) throw std::invalid_argument("invalid padding");
  for (size_t i = 0; i < pad; ++i) {
    if (data[data.size() - 1 - i] != pad) throw std::invalid_argument("invalid padding");
  }
  return std::vector<unsigned char>(data.begin(), data.end() - pad);
}

} // namespace pkcs7
