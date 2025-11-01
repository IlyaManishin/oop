#pragma once

#include <climits>
#include <cstddef>

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

        typedef char sign;
        typedef uint32_t digit;
        typedef uint64_t twodigits;

        constexpr size_t BL_DIGIT_OFFSET = 2;
        constexpr size_t BL_BIT_COUNT = sizeof(digit) * 8 - BL_DIGIT_OFFSET;
        constexpr size_t BL_DIGIT_MOD = 1ull << BL_BIT_COUNT;
        constexpr size_t BL_DIGIT_MASK = BL_DIGIT_MOD - 1;


        constexpr size_t log10_ceil(uint64_t n)
        {
            size_t res = 0;
            uint64_t val = 1;
            while (val <= n)
            {
                val *= 10;
                res++;
            }
            return res - 1;
        }

        constexpr size_t npow(size_t a, size_t n)
        {
            size_t res = 1;
            for (size_t i = 0; i < n; i++)
                res *= a;
            return res;
        }

        constexpr size_t BL_DEC_IN_DIGIT = log10_ceil(BL_DIGIT_MOD) - 5;
        constexpr size_t BL_DEC_PART_DIV = npow(10, BL_DEC_IN_DIGIT);
        // constexpr size_t BL_DEC_IN_DIGIT = 1;
        // constexpr size_t BL_DEC_PART_DIV = 10;

    } // namespace _detail

} // namespace bigLong
