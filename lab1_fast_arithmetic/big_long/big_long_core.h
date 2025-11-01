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

        void abs_digits_add_to_first(std::vector<digit> &num1,
                                     const std::vector<digit> &num2);

        std::vector<digit> abs_digits_add(const std::vector<digit> &num1,
                                          const std::vector<digit> &num2);

        std::vector<digit> abs_digits_mul(const std::vector<digit> &num1,
                                          const std::vector<digit> &num2);

        const char *extract_sign_from_string(const char *start, const char *end, sign *signDest);
        digit extract_digit_from_str(const char *start, const char *end);
    } // namespace _detail

} // namespace bigLong
