#pragma once

#include <cstdint>

namespace bigLong
{
    namespace _detail
    {
        enum SIGN
        {
            POSITIVE_NUM = 1,
            ZERO_NUM = 0,
            NEGATIVE_NUM = -1,
        };
        typedef uint32_t digit;
        typedef uint64_t twodigits;
        typedef char sign;
    } // namespace _detail
    
} // namespace bigLong
