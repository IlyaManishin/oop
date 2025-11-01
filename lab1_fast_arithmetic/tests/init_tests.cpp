#include <gtest/gtest.h>
#include <string>

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

TEST(BigLongConstructorTest, VeryLongString0)
{
    std::string numStr = "12345678900";
    BigLong check(numStr);
    check /= 100;
    BigLong valid(123456789);

    EXPECT_TRUE(check == valid);
}

TEST(BigLongConstructorTest, VeryLongString1)
{
    std::string numStr = "1234567890";
    int zeros = 100;
    BigLong valid(numStr);
    for (int i = 0; i < zeros; i++)
    {
        numStr += std::string("0");
        valid *= 10;
    }
    BigLong check(numStr);
    EXPECT_TRUE(check == valid);
}

TEST(BigLongConstructorTest, CheckMulty)
{
    std::string numStr = "1234";
    int zeros =  10;
    BigLong check(1234);
    for (int i = 0; i < zeros; i++)
    {
        numStr += std::string("0000000000");
        check *= 10000000000;
    }
    BigLong valid(numStr);
    EXPECT_TRUE(check == valid);
}

TEST(BigLongConvertToStringTest, PositiveNumber)
{
    BigLong a(123456789);
    EXPECT_EQ(a.convertToString(), "123456789");
}

TEST(BigLongConvertToStringTest, NegativeNumber)
{
    BigLong a(-987654321);
    EXPECT_EQ(a.convertToString(), "-987654321");
}

TEST(BigLongConvertToStringTest, Zero)
{
    BigLong a(0);
    EXPECT_EQ(a.convertToString(), "0");
}


