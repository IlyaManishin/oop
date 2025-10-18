#pragma once

#include <climits>
#include <cstddef>

#include "big_long_types.h"

namespace bigLong
{
    namespace _detail
    {
        enum SIGNS
        {
            POSITIVE_NUM = 1,
            ZERO_NUM = 0,
            NEGATIVE_NUM = -1,
        };

        constexpr size_t BL_DIGIT_OFFSET = 2;
        constexpr size_t BL_BIT_COUNT = sizeof(digit) * 8 - BL_DIGIT_OFFSET;
        constexpr size_t BL_DIGIT_MOD = 1ull << (BL_BIT_COUNT + 1);
        constexpr size_t BL_DIGIT_MASK = BL_DIGIT_MOD - 1;

        constexpr size_t BL_BASE_DIGITS_COUNT = 1;
        constexpr size_t BL_MAX_DIGITS = INT32_MAX >> 1;

    } // namespace _detail

} // namespace bigLong
