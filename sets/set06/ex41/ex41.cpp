#include "ex41.h"

#include <stdexcept>

namespace set06::ex41
{

    DecryptionOracle::DecryptionOracle(const set05::ex39::PrivateKey &private_key)
        : private_key_(private_key)
    {
    }

    bignum::BigUint DecryptionOracle::decrypt_once(const bignum::BigUint &ciphertext)
    {
        if (!seen_ciphertexts_.insert(ciphertext.to_hex()).second)
            throw std::invalid_argument("ciphertext has already been decrypted");
        return set05::ex39::decrypt(ciphertext, private_key_);
    }

    bignum::BigUint recover_plaintext(
        const bignum::BigUint &ciphertext,
        const set05::ex39::PublicKey &public_key,
        DecryptionOracle &oracle,
        const bignum::BigUint &blinding_factor
    )
    {
        if (blinding_factor <= bignum::BigUint(1) ||
            blinding_factor >= public_key.n ||
            bignum::gcd(blinding_factor, public_key.n) != bignum::BigUint(1))
            throw std::invalid_argument("blinding factor must be invertible modulo n and greater than one");

        const auto multiplier = bignum::mod_pow(
            blinding_factor,
            public_key.e,
            public_key.n
        );
        const auto blinded_ciphertext = (ciphertext * multiplier) % public_key.n;
        if (blinded_ciphertext == ciphertext)
            throw std::invalid_argument("blinding factor did not change the ciphertext");
        const auto blinded_plaintext = oracle.decrypt_once(blinded_ciphertext);
        return (blinded_plaintext * bignum::mod_inverse(blinding_factor, public_key.n)) %
               public_key.n;
    }

} // namespace set06::ex41
