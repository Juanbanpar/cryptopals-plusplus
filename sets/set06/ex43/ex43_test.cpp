#include <gtest/gtest.h>

#include "ex43.h"

TEST(Ex43, SignsAndVerifiesWithChallengeParameters)
{
    const auto parameters = set06::dsa::challenge_parameters();
    const auto keys = set06::dsa::key_pair_from_private(parameters, bignum::BigUint(17));
    const std::vector<unsigned char> message{'h', 'e', 'l', 'l', 'o'};
    const auto signature = set06::dsa::sign(message, keys.private_key, bignum::BigUint(19));
    EXPECT_TRUE(set06::dsa::verify(message, signature, keys.public_key));
    EXPECT_FALSE(set06::dsa::verify(
        std::vector<unsigned char>{'n', 'o'}, signature, keys.public_key
    ));
}

TEST(Ex43, RecoversChallengePrivateKeyFromSmallNonce)
{
    const auto recovered = set06::ex43::recover_key_from_small_nonce(
        set06::ex43::challenge_message(),
        set06::ex43::challenge_signature(),
        set06::ex43::challenge_public_key()
    );
    ASSERT_TRUE(recovered.found);
    EXPECT_EQ(
        set06::dsa::private_key_fingerprint(recovered.private_key),
        "0954edd5e0afe5542a4adf012611a91912a3ec16"
    );
}
