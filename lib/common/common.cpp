#include "common.h"
#include <stdexcept>

#include <cctype>
#include <limits>
#include <random>
#include <set>
#include <functional>

std::vector<unsigned char> rand_bytes(size_t n) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> dis(0, 255);
  std::vector<unsigned char> out(n);
  for (size_t i = 0; i < n; ++i) out[i] = static_cast<unsigned char>(dis(gen));
  return out;
}

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

// bytes_to_base64 and base64_to_bytes are implemented in lib/base64
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

double score_english(const std::string &s) {
  static const std::string letters = " etaoinshrdlcumwfgypbvkjxqz";
  static double table[256];
  static bool init = false;
  if (!init) {
    init = true;
    for (int i = 0; i < 256; ++i) table[i] = 0.0;
    for (size_t i = 0; i < letters.size(); ++i) table[(unsigned char)letters[i]] = (double)(letters.size() - i);
  }
  double score = 0.0;
  for (unsigned char c : s) {
    if (c >= 32 && c <= 126) {
      unsigned char lc = static_cast<unsigned char>(std::tolower(c));
      score += table[lc];
    } else {
      score -= 20.0;
    }
  }
  return score;
}

std::pair<unsigned char, std::string> crack_single_byte_xor_hex(const std::string &hex) {
  auto bytes = hex_to_bytes(hex);
  double best_score = -std::numeric_limits<double>::infinity();
  unsigned char best_key = 0;
  std::string best_plain;
  for (int k = 0; k < 256; ++k) {
    std::string out;
    out.resize(bytes.size());
    for (size_t i = 0; i < bytes.size(); ++i) out[i] = static_cast<char>(bytes[i] ^ (unsigned char)k);
    double sc = score_english(out);
    if (sc > best_score) {
      best_score = sc;
      best_key = static_cast<unsigned char>(k);
      best_plain = out;
    }
  }
  return {best_key, best_plain};
}

// base64_to_bytes implemented in lib/base64

int hamming_distance(const std::vector<unsigned char> &a, const std::vector<unsigned char> &b) {
  if (a.size() != b.size()) throw std::invalid_argument("hamming: inputs must have equal length");
  int dist = 0;
  for (size_t i = 0; i < a.size(); ++i) {
    unsigned char x = a[i] ^ b[i];
    dist += __builtin_popcount((unsigned int)x);
  }
  return dist;
}

std::pair<unsigned char, std::string> crack_single_byte_xor_bytes(const std::vector<unsigned char> &bytes) {
  double best_score = -std::numeric_limits<double>::infinity();
  unsigned char best_key = 0;
  std::string best_plain;
  for (int k = 0; k < 256; ++k) {
    std::string out;
    out.resize(bytes.size());
    for (size_t i = 0; i < bytes.size(); ++i) out[i] = static_cast<char>(bytes[i] ^ (unsigned char)k);
    double sc = score_english(out);
    if (sc > best_score) {
      best_score = sc;
      best_key = static_cast<unsigned char>(k);
      best_plain = out;
    }
  }
  return {best_key, best_plain};
}

// Detect whether an encryption oracle is using ECB or CBC mode.
// The oracle should accept plaintext bytes and return ciphertext bytes.
std::string detect_ecb_or_cbc(const std::function<std::vector<unsigned char>(const std::vector<unsigned char>&)>& oracle) {
  const size_t block_size = 16;
  std::vector<unsigned char> payload(block_size * 4, 'A');
  auto ciphertext = oracle(payload);

  std::set<std::string> blocks;
  for (size_t i = 0; i + block_size <= ciphertext.size(); i += block_size) {
    std::string block(ciphertext.begin() + i, ciphertext.begin() + i + block_size);
    if (blocks.count(block)) return "ECB";
    blocks.insert(block);
  }
  return "CBC";
}
