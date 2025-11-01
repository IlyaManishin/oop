#include <gtest/gtest.h>

#include "../big_long/big_long.h"
#include "../big_long/big_long_core.h"
#include "../big_long/big_long_config.h"

using namespace bigLong;
using namespace bigLong::_detail;


TEST(ExtractDigitFromStrTest, Basic)
{
    const char *s = "12345";
    digit result = extract_digit_from_str(s, s + 5);
    EXPECT_EQ(result, 12345);
}

TEST(ExtractDigitFromStrTest, LeadingZeros)
{
    const char *s = "00042";
    digit result = extract_digit_from_str(s, s + 5);
    EXPECT_EQ(result, 42);
}

TEST(ExtractDigitFromStrTest, SingleDigit)
{
    const char *s = "7";
    digit result = extract_digit_from_str(s, s + 1);
    EXPECT_EQ(result, 7);
}
