#pragma once

#include <cstddef>

namespace bigLong
{
    constexpr int digits_size(size_t byteSize)
    {
        return byteSize % BL_USED_DIGIT_BITS == 0 ? byteSize / BL_USED_DIGIT_BITS : byteSize / BL_USED_DIGIT_BITS + 1;
    }
} // namespace bigLong
