#pragma once

#include <cstddef>
#include <vector>

#include "big_long_types.h"

namespace bigLong
{
    using namespace _detail;

    constexpr size_t type_digits_size(size_t byteSize);

    std::vector<digit> abs_digits_sub(const std::vector<digit> &num1,
                               const std::vector<digit> &num2);

    std::vector<digit> abs_digits_add(const std::vector<digit> &num1,
                               const std::vector<digit> &num2);

} // namespace bigLong
