#include <gtest/gtest.h>
#include "ex26.h"

TEST(Ex26, CTRBitflipping)
{
    const auto clean = set04::ex26::encrypt_userdata(";admin=true;");
    EXPECT_FALSE(set04::ex26::is_admin(clean));
    EXPECT_TRUE(set04::ex26::is_admin(set04::ex26::create_admin_ciphertext()));
}
