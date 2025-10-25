#include <gtest/gtest.h>

#include "../big_long/big_long.h"

using namespace bigLong;

TEST(BigLongConstructorTest, FromIntegral1)
{
    BigLong a(123);
    EXPECT_TRUE(a == 123);
}

TEST(BigLongConstructorTest, FromIntegral2)
{
    BigLong a(100000);
    a += 100000;
    EXPECT_TRUE(a == 200000);
}

TEST(BigLongConstructorTest, FromFloating)
{
    BigLong a(123.99);
    EXPECT_TRUE(a == 123);
}

TEST(BigLongConstructorTest, FromString)
{
    BigLong a("456");
    EXPECT_TRUE(a == 456);
}

TEST(BigLongConstructorTest, FromCString)
{
    const char *num = "789";
    BigLong a(num);
    EXPECT_TRUE(a == 789);
}
