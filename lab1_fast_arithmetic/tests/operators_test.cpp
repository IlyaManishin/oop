#include "../big_long/big_long.h"
#include <gtest/gtest.h>
#include <stdexcept>

using namespace bigLong;

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

TEST(BigLongOperatorTest, CheckBigValuesDiv)
{
    BigLong a(1), b(1);
    int size_count = 20;

    for (int i = 0; i < size_count; i++)
    {
        a *= 2;
        b += b;
    }

    for (int i = 0; i < size_count; i++)
    {
        a /= 2;
        b /= 2;
    }

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a == 1);
}

TEST(BigLongOperatorTest, DivisionByOne)
{
    BigLong a(123456789);
    BigLong b(1);
    a /= b;
    EXPECT_TRUE(a == 123456789);
}

TEST(BigLongOperatorTest, DivisionOfEqualNumbers)
{
    BigLong a(777);
    BigLong b(777);
    a /= b;
    EXPECT_TRUE(a == 1);
}

TEST(BigLongOperatorTest, DivisionSignCheck)
{
    BigLong a(-100);
    BigLong b(20);
    a /= b;
    EXPECT_TRUE(a == -5);
}

TEST(BigLongOperatorTest, DivisionZeroCase)
{
    BigLong a(0);
    BigLong b(123);
    a /= b;
    EXPECT_TRUE(a == 0);
}

TEST(BigLongOperatorTest, DivisionByZero)
{
    BigLong a(12345);
    BigLong zero(0);

    ASSERT_THROW(a /= zero, std::runtime_error);
    ASSERT_THROW(a / zero, std::runtime_error);
}

TEST(BigLongOperatorTest, DivisionTerm)
{
    BigLong a(0);
    BigLong b = (a + 100) / 10;
    ASSERT_TRUE(b == 10);
}
