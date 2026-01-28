#include "ex09.h"
#include <stdexcept>

namespace set02::ex09 {

std::vector<unsigned char> pkcs7_pad(const std::vector<unsigned char> &data, size_t block_size) {
  if (block_size == 0) throw std::invalid_argument("block_size must be > 0");
  size_t pad_len = block_size - (data.size() % block_size);
  if (pad_len == 0) pad_len = block_size;
  std::vector<unsigned char> out = data;
  out.insert(out.end(), pad_len, static_cast<unsigned char>(pad_len));
  return out;
}

std::string pkcs7_pad_string(const std::string &s, size_t block_size) {
  std::vector<unsigned char> data(s.begin(), s.end());
  auto padded = pkcs7_pad(data, block_size);
  return std::string(padded.begin(), padded.end());
}

} // namespace set02::ex09
