#include "common.h"
#include <stdexcept>

std::vector<unsigned char> hex_to_bytes(const std::string &hex) {
  std::vector<unsigned char> out;
  if (hex.size() % 2 != 0) throw std::invalid_argument("hex string has odd length");
  auto hexval = [](char c) -> int {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
  };
  out.reserve(hex.size() / 2);
  for (size_t i = 0; i < hex.size(); i += 2) {
    int hi = hexval(hex[i]);
    int lo = hexval(hex[i + 1]);
    if (hi < 0 || lo < 0) throw std::invalid_argument("invalid hex digit");
    out.push_back(static_cast<unsigned char>((hi << 4) | lo));
  }
  return out;
}

std::string bytes_to_base64(const std::vector<unsigned char> &data) {
  static const char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  std::string out;
  size_t i = 0;
  while (i + 2 < data.size()) {
    unsigned int val = (data[i] << 16) | (data[i+1] << 8) | data[i+2];
    out.push_back(table[(val >> 18) & 0x3F]);
    out.push_back(table[(val >> 12) & 0x3F]);
    out.push_back(table[(val >> 6) & 0x3F]);
    out.push_back(table[val & 0x3F]);
    i += 3;
  }
  size_t rem = data.size() - i;
  if (rem == 1) {
    unsigned int val = (data[i] << 16);
    out.push_back(table[(val >> 18) & 0x3F]);
    out.push_back(table[(val >> 12) & 0x3F]);
    out.push_back('=');
    out.push_back('=');
  } else if (rem == 2) {
    unsigned int val = (data[i] << 16) | (data[i+1] << 8);
    out.push_back(table[(val >> 18) & 0x3F]);
    out.push_back(table[(val >> 12) & 0x3F]);
    out.push_back(table[(val >> 6) & 0x3F]);
    out.push_back('=');
  }
  return out;
}

std::string hex_to_base64(const std::string &hex) {
  auto bytes = hex_to_bytes(hex);
  return bytes_to_base64(bytes);
}

std::string bytes_to_hex(const std::vector<unsigned char> &data) {
  static const char table[] = "0123456789abcdef";
  std::string out;
  out.reserve(data.size() * 2);
  for (unsigned char b : data) {
    out.push_back(table[(b >> 4) & 0xF]);
    out.push_back(table[b & 0xF]);
  }
  return out;
}

std::string fixed_xor_hex(const std::string &ahex, const std::string &bhex) {
  auto a = hex_to_bytes(ahex);
  auto b = hex_to_bytes(bhex);
  if (a.size() != b.size()) throw std::invalid_argument("inputs must have equal length");
  std::vector<unsigned char> out;
  out.reserve(a.size());
  for (size_t i = 0; i < a.size(); ++i) out.push_back(static_cast<unsigned char>(a[i] ^ b[i]));
  return bytes_to_hex(out);
}
