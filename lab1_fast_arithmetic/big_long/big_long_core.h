#pragma once

#include <cstddef>
#include <vector>

#include "big_long_config.h"

namespace bigLong
{
    namespace _detail
    {
        std::vector<digit> abs_digits_sub(const std::vector<digit> &num1,
                                          const std::vector<digit> &num2);

        std::vector<digit> abs_digits_add(const std::vector<digit> &num1,
                                          const std::vector<digit> &num2);

        std::vector<digit> abs_digits_mul(const std::vector<digit> &num1,
                                          const std::vector<digit> &num2);
    } // namespace _detail

} // namespace bigLong
