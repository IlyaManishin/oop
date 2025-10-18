#pragma once

#include <cstdint>

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
    } // namespace _detail
    
} // namespace bigLong
