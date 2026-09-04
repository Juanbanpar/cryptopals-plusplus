#include "ex25.h"
#include "aes.h"
#include "common.h"
#include <fstream>
#include <stdexcept>

namespace set04::ex25
{

    namespace
    {
        std::vector<unsigned char> KEY;
        bool initialized = false;

        void ensure_init()
        {
            if (!initialized)
            {
                KEY = rand_bytes(16);
                initialized = true;
            }
        }

        std::vector<unsigned char> load_challenge_plaintext()
        {
            const std::string path = std::string(PROJECT_SOURCE_DIR) + "/resources/set04/ex25/25.txt";
            std::ifstream ifs(path);
            if (!ifs)
                throw std::runtime_error("Could not open challenge 25 data: " + path);

            std::string encoded;
            std::string line;
            while (std::getline(ifs, line))
                encoded += line;

            const auto ciphertext = base64_to_bytes(encoded);
            const std::vector<unsigned char> key{'Y', 'E', 'L', 'L', 'O', 'W', ' ', 'S',
                                                 'U', 'B', 'M', 'A', 'R', 'I', 'N', 'E'};
            return aes::decrypt_ecb(ciphertext, key);
        }
    }

    std::vector<unsigned char> encrypt_challenge_plaintext()
    {
        ensure_init();
        return aes::ctr(load_challenge_plaintext(), KEY, 0);
    }

    std::vector<unsigned char> edit_ciphertext(
        const std::vector<unsigned char> &ciphertext,
        const std::vector<unsigned char> &key,
        size_t offset,
        const std::vector<unsigned char> &newtext
    )
    {
        if (offset > ciphertext.size() || newtext.size() > ciphertext.size() - offset)
            throw std::invalid_argument("CTR edit range is outside the ciphertext");

        auto plaintext = aes::ctr(ciphertext, key, 0);
        for (size_t i = 0; i < newtext.size(); ++i)
            plaintext[offset + i] = newtext[i];
        return aes::ctr(plaintext, key, 0);
    }

    std::vector<unsigned char> edit_oracle(
        const std::vector<unsigned char> &ciphertext,
        size_t offset,
        const std::vector<unsigned char> &newtext
    )
    {
        ensure_init();
        return edit_ciphertext(ciphertext, KEY, offset, newtext);
    }

    std::vector<unsigned char> recover_plaintext(const std::vector<unsigned char> &ciphertext)
    {
        const auto edited = edit_oracle(ciphertext, 0, std::vector<unsigned char>(ciphertext.size(), 0));
        std::vector<unsigned char> plaintext(ciphertext.size());
        for (size_t i = 0; i < ciphertext.size(); ++i)
            plaintext[i] = ciphertext[i] ^ edited[i];
        return plaintext;
    }

} // namespace set04::ex25
