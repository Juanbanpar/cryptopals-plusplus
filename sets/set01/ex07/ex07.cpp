#include "ex07.h"
#include "../../lib/common/common.h"
#include <openssl/evp.h>

#include <fstream>
#include <stdexcept>

namespace {
std::vector<unsigned char> aes_128_ecb_decrypt(const std::vector<unsigned char> &cipher, const std::string &key) {
  if (key.size() != 16) throw std::invalid_argument("key must be 16 bytes for AES-128");
  EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
  if (!ctx) throw std::runtime_error("EVP_CIPHER_CTX_new failed");
  int rc = EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, (const unsigned char*)key.data(), NULL);
  if (rc != 1) { EVP_CIPHER_CTX_free(ctx); throw std::runtime_error("EVP_DecryptInit_ex failed"); }
  EVP_CIPHER_CTX_set_padding(ctx, 1);
  std::vector<unsigned char> out(cipher.size() + 16);
  int outlen1 = 0;
  if (!EVP_DecryptUpdate(ctx, out.data(), &outlen1, cipher.data(), (int)cipher.size())) {
    EVP_CIPHER_CTX_free(ctx); throw std::runtime_error("EVP_DecryptUpdate failed");
  }
  int outlen2 = 0;
  if (!EVP_DecryptFinal_ex(ctx, out.data() + outlen1, &outlen2)) {
    EVP_CIPHER_CTX_free(ctx); throw std::runtime_error("EVP_DecryptFinal_ex failed (bad padding?)");
  }
  EVP_CIPHER_CTX_free(ctx);
  out.resize(outlen1 + outlen2);
  return out;
}
}

std::string set01::ex07::decrypt_aes_128_ecb_base64(const std::vector<unsigned char> &data, const std::string &key) {
  auto plain_bytes = aes_128_ecb_decrypt(data, key);
  return std::string(plain_bytes.begin(), plain_bytes.end());
}

std::string set01::ex07::decrypt_aes_128_ecb_base64_file(const std::string &path, const std::string &key) {
  std::ifstream ifs(path);
  if (!ifs) return std::string();
  std::string line, b64;
  while (std::getline(ifs, line)) b64 += line;
  auto cipher = base64_to_bytes(b64);
  return decrypt_aes_128_ecb_base64(cipher, key);
}
