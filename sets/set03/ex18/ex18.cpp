#include "ex18.h"
#include "common.h"
#include "aes.h"

namespace set03::ex18
{

    std::string solve_challenge()
    {
        const std::string ciphertext_b64 = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ==";
        auto ciphertext = base64_to_bytes(ciphertext_b64);

        const std::string key_str = "YELLOW SUBMARINE";
        std::vector<uint8_t> key(key_str.begin(), key_str.end());
        uint64_t nonce = 0;

        auto plaintext = aes::ctr(ciphertext, key, nonce);
        return std::string(plaintext.begin(), plaintext.end());
    }

} // namespace set03::ex18
