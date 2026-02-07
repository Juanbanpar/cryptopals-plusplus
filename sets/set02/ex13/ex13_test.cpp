#include <gtest/gtest.h>
#include "ex13.h"

TEST(Ex13, ParseKv)
{
    auto kv = set02::ex13::parse_kv("foo=bar&baz=qux&zap=zazzle");
    EXPECT_EQ(kv["foo"], "bar");
    EXPECT_EQ(kv["baz"], "qux");
    EXPECT_EQ(kv["zap"], "zazzle");
}

TEST(Ex13, EncryptDecrypt)
{
    auto cipher = set02::ex13::encrypt_profile_for("foo@bar.com");
    auto p = set02::ex13::decrypt_profile(cipher);
    EXPECT_EQ(p.email, "foo@bar.com");
    EXPECT_EQ(p.uid, 10);
    EXPECT_EQ(p.role, "user");
}

TEST(Ex13, CutAndPaste)
{
    auto cipher = set02::ex13::create_admin_profile();
    auto p = set02::ex13::decrypt_profile(cipher);
    EXPECT_EQ(p.role, "admin");
}
