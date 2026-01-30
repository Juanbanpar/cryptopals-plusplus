#include <gtest/gtest.h>
#include "ex16.h"

TEST(BitFlipping, Example) {
    auto cipher = set02::ex16::create_admin_ciphertext();
    // Clean encryption shouldn't be admin
    auto clean = set02::ex16::encrypt_userdata("admin=true"); // would be quoted
    EXPECT_FALSE(set02::ex16::is_admin(clean));
    
    EXPECT_TRUE(set02::ex16::is_admin(cipher));
}
