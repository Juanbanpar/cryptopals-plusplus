#include "common.h"
#include <stdexcept>

#include <cctype>
#include <limits>


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

std::vector<unsigned char> base64_to_bytes(const std::string &b64) {
  static const int T[256] = {
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
    52,53,54,55,56,57,58,59,60,61,-1,-1,-1, 0,-1,-1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
    15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
    -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
    41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1
  };
  std::vector<int> vals;
  vals.reserve(b64.size());
  int pad = 0;
  for (unsigned char c : b64) {
    if (c == '=') { pad++; continue; }
    int v = T[c];
    if (v >= 0) vals.push_back(v);
  }
  std::vector<unsigned char> out;
  for (size_t i = 0; i + 3 < vals.size(); i += 4) {
    int v = (vals[i] << 18) | (vals[i+1] << 12) | (vals[i+2] << 6) | vals[i+3];
    out.push_back(static_cast<unsigned char>((v >> 16) & 0xFF));
    out.push_back(static_cast<unsigned char>((v >> 8) & 0xFF));
    out.push_back(static_cast<unsigned char>(v & 0xFF));
  }
  // handle leftover groups (if any)
  size_t rem = vals.size() % 4;
  if (rem == 2) {
    int v = (vals[vals.size()-2] << 18) | (vals[vals.size()-1] << 12);
    out.push_back(static_cast<unsigned char>((v >> 16) & 0xFF));
  } else if (rem == 3) {
    int v = (vals[vals.size()-3] << 18) | (vals[vals.size()-2] << 12) | (vals[vals.size()-1] << 6);
    out.push_back(static_cast<unsigned char>((v >> 16) & 0xFF));
    out.push_back(static_cast<unsigned char>((v >> 8) & 0xFF));
  }
  // trim padding if present
  if (pad > 0 && pad <= 2) out.resize(out.size() - pad);
  return out;
}

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
