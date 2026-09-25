#pragma once

#include "ex39.h"

#include <set>
#include <string>

namespace set06::ex41
{

    class DecryptionOracle
    {
    public:
        explicit DecryptionOracle(const set05::ex39::PrivateKey &private_key);
        bignum::BigUint decrypt_once(const bignum::BigUint &ciphertext);

    private:
        set05::ex39::PrivateKey private_key_;
        std::set<std::string> seen_ciphertexts_;
    };

    bignum::BigUint recover_plaintext(
        const bignum::BigUint &ciphertext,
        const set05::ex39::PublicKey &public_key,
        DecryptionOracle &oracle,
        const bignum::BigUint &blinding_factor
    );

} // namespace set06::ex41
