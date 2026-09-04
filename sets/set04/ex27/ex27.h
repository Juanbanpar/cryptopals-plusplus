#pragma once

#include <stdexcept>
#include <vector>

namespace set04::ex27
{

    class InvalidAsciiError : public std::runtime_error
    {
    public:
        explicit InvalidAsciiError(const std::vector<unsigned char> &plaintext);

        const std::vector<unsigned char> &plaintext() const noexcept;

    private:
        std::vector<unsigned char> plaintext_;
    };

    // Encrypt with AES-CBC using the same random key as the IV.
    std::vector<unsigned char> encrypt_message(const std::vector<unsigned char> &plaintext);

    // Decrypt, reject high-ASCII plaintext, and remove PKCS#7 padding.
    std::vector<unsigned char> decrypt_message(const std::vector<unsigned char> &ciphertext);

    // Recover the CBC key by provoking an invalid-ASCII decryption error.
    std::vector<unsigned char> recover_key(const std::vector<unsigned char> &ciphertext);

} // namespace set04::ex27
