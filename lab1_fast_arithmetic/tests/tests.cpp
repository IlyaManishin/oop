#include "../big_long/big_long.h"
#include <gtest/gtest.h>

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

TEST(BigLongOperatorTest, PlusWithIntegral)
{
    BigLong a(10);
    BigLong result = a + 5;
    EXPECT_TRUE(result == 15);
}

TEST(BigLongOperatorTest, PlusWithBigLong)
{
    BigLong a(10), b(20);
    BigLong result = a + b;
    EXPECT_TRUE(result == 30);
}

TEST(BigLongOperatorTest, PlusAssignIntegral)
{
    BigLong a(10);
    a += 7;
    EXPECT_TRUE(a == 17);
}

TEST(BigLongOperatorTest, PlusAssignBigLong)
{
    BigLong a(10), b(7);
    a += b;
    EXPECT_TRUE(a == 17);
}

TEST(BigLongOperatorTest, MinusWithIntegral)
{
    BigLong a(10);
    BigLong result = a - 3;
    EXPECT_TRUE(result == 7);
}

TEST(BigLongOperatorTest, MinusWithBigLong)
{
    BigLong a(20), b(8);
    BigLong result = a - b;
    EXPECT_TRUE(result == 12);
}

TEST(BigLongOperatorTest, MinusAssignIntegral)
{
    BigLong a(15);
    a -= 4;
    EXPECT_TRUE(a == 11);
}

TEST(BigLongOperatorTest, MinusAssignBigLong)
{
    BigLong a(15), b(4);
    a -= b;
    EXPECT_TRUE(a == 11);
}

TEST(BigLongOperatorTest, MixedTypes)
{
    BigLong a(100);
    EXPECT_TRUE((a + 25.0) == 125);
    EXPECT_TRUE((a - 30.0) == 70);
}

TEST(BigLongOperatorTest, CheckBigValues)
{
    BigLong a(1), b(1);
    int size_count = 1000000;
    for (int i = 0; i < size_count; i++)
    {
        a += 10000000;
    }
    for (int i = 0; i < size_count / 10; i++)
    {
        b += 100000000;
    }
    EXPECT_TRUE(a == b);
}

TEST(BigLongOperatorTest, CheckBigValuesMul)
{
    BigLong a(1), b(1);
    int size_count = 100000;

    for (int i = 0; i < size_count; i++)
    {
        a *= 2;
    }
    for (int i = 0; i < size_count; i++)
    {
        b += b;
    }
    EXPECT_TRUE(a == b);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}