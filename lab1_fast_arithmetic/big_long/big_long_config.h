#pragma once

#include <cstddef>
#include <climits>

#include "big_long_types.h"

namespace bigLong
{
    namespace _detail
    {
        constexpr size_t BL_DIGIT_OFFSET = 2;
        constexpr size_t BL_USED_DIGIT_BITS = sizeof(digit) - BL_DIGIT_OFFSET;
        constexpr size_t BL_DIGIT_MOD = 1ull << (BL_USED_DIGIT_BITS + 1);
        constexpr size_t BL_DIGIT_MASK = BL_DIGIT_MOD - 1;
    
        constexpr size_t BL_BASE_DIGITS_COUNT = 1;
        constexpr size_t BL_MAX_DIGITS = INT32_MAX >> 1;
        
    } // namespace _detail
    
} // namespace bigLong
