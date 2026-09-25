#include "ex42.h"

#include "sha1.h"

#include <algorithm>
#include <stdexcept>

namespace set06::ex42
{

    namespace
    {
        const std::vector<unsigned char> SHA1_DIGEST_INFO = {
            0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e,
            0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14};

        std::vector<unsigned char> expected_prefix(
            const std::vector<unsigned char> &message
        )
        {
            auto result = SHA1_DIGEST_INFO;
            const auto digest = sha1::digest(message);
            result.insert(result.end(), digest.begin(), digest.end());
            return result;
        }
    }

    bignum::BigUint forge_signature(
        const std::vector<unsigned char> &message,
        const set05::ex39::PublicKey &public_key,
        size_t block_size
    )
    {
        const auto digest_info = expected_prefix(message);
        if (block_size < digest_info.size() + 4)
            throw std::invalid_argument("RSA block is too short for a SHA-1 signature");

        // The verifier ignores bytes after the digest. Filling this suffix
        // with 0xff keeps the cube root's rounding error inside that suffix;
        // zeroes would make the subtraction borrow into the digest itself.
        std::vector<unsigned char> block(block_size, 0xff);
        block[0] = 0x00;
        block[1] = 0x01;
        block[2] = 0xff;
        block[3] = 0x00;
        std::copy(digest_info.begin(), digest_info.end(), block.begin() + 4);
        const auto target = bignum::BigUint::from_bytes_be(block);
        if (target >= public_key.n)
            throw std::invalid_argument("signature block must be smaller than the RSA modulus");

        const auto signature = bignum::integer_cube_root(target);
        return signature;
    }

    bool verify_signature_weakly(
        const std::vector<unsigned char> &message,
        const bignum::BigUint &signature,
        const set05::ex39::PublicKey &public_key,
        size_t block_size
    )
    {
        if (signature >= public_key.n || public_key.e != bignum::BigUint(3))
            return false;

        const auto encoded = set05::ex39::integer_to_bytes(
            bignum::mod_pow(signature, public_key.e, public_key.n),
            block_size
        );
        if (encoded.size() < 4 || encoded[0] != 0x00 || encoded[1] != 0x01)
            return false;

        size_t separator = 2;
        while (separator < encoded.size() && encoded[separator] == 0xff)
            ++separator;
        if (separator == 2 || separator >= encoded.size() || encoded[separator] != 0x00)
            return false;

        const auto prefix = expected_prefix(message);
        const size_t digest_start = separator + 1;
        if (encoded.size() - digest_start < prefix.size())
            return false;
        return std::equal(
            prefix.begin(),
            prefix.end(),
            encoded.begin() + static_cast<std::ptrdiff_t>(digest_start)
        );
    }

} // namespace set06::ex42
