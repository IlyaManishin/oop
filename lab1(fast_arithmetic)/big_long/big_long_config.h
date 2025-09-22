#pragma once

#include <cstddef>

#include "big_long.h"

namespace bigLong
{
    constexpr size_t BL_DIGIT_OFFSET = 2;
    constexpr size_t BL_USED_DIGIT_BITS = sizeof(detail::digit) - BL_DIGIT_OFFSET;
    constexpr size_t BL_DIGIT_MASK = (1ull << (BL_USED_DIGIT_BITS + 1)) - 1;


} // namespace bigLong
