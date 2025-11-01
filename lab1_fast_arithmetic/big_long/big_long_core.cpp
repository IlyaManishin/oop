#include <cassert>
#include <cmath>
#include <vector>

#include "big_long.h"
#include "big_long_config.h"
#include "big_long_core.h"

namespace bigLong
{
    namespace _detail
    {
        void abs_digits_add_to_first(std::vector<digit> &num1, const std::vector<digit> &num2)
        {
            size_t minSize = std::min(num1.size(), num2.size());
            size_t maxSize = std::max(num1.size(), num2.size());

            const std::vector<digit> *largest = &num1;
            if (num1.size() < maxSize)
            {
                num1.resize(maxSize, 0);
                largest = &num2;
            }

            digit carry = 0, sum = 0;
            size_t i = 0;

            for (; i < minSize; ++i)
            {
                sum = num1[i] + num2[i] + carry;
                num1[i] = sum & BL_DIGIT_MASK;
                carry = sum >> BL_BIT_COUNT;
            }

            for (; i < maxSize; ++i)
            {
                digit sum = (*largest)[i] + carry;
                num1[i] = sum & BL_DIGIT_MASK;
                carry = sum >> BL_BIT_COUNT;
            }

            if (carry)
                num1.push_back(carry);
        }

        std::vector<digit> abs_digits_add(const std::vector<digit> &num1, const std::vector<digit> &num2)
        {
            size_t maxSize = std::max(num1.size(), num2.size());
            std::vector<digit> result(num1);
            result.resize(maxSize + 1, static_cast<digit>(0));
            abs_digits_add_to_first(result, num2);
            return result;
        }

        std::vector<digit> abs_digits_sub(const std::vector<digit> &num1, const std::vector<digit> &num2)
        {
            std::vector<digit> result(num1.size(), 0);
            digit borrow = 0;
            size_t i = 0;

            for (; i < num2.size(); ++i)
            {
                int32_t diff = (digit)num1[i] - num2[i] - borrow;
                if (diff < 0)
                {
                    diff += BL_DIGIT_MOD;
                    borrow = 1;
                }
                else
                {
                    borrow = 0;
                }
                result[i] = static_cast<digit>(diff);
            }

            for (; i < num1.size(); ++i)
            {
                int32_t diff = (digit)num1[i] - borrow;
                if (diff < 0)
                {
                    diff += BL_DIGIT_MOD;
                    borrow = 1;
                }
                else
                {
                    borrow = 0;
                }
                result[i] = static_cast<digit>(diff);
            }
            return result;
        }

        std::vector<digit> abs_digits_mul(const std::vector<digit> &num1,
                                          const std::vector<digit> &num2)
        {
            size_t size_a = num1.size();
            size_t size_b = num2.size();
            std::vector<digit> result(size_a + size_b, 0);

            for (size_t i = 0; i < size_a; ++i)
            {
                twodigits carry = 0;
                twodigits f = num1[i];

                for (size_t j = 0; j < size_b; ++j)
                {
                    twodigits tmp = static_cast<twodigits>(result[i + j]) + f * num2[j] + carry;
                    result[i + j] = static_cast<digit>(tmp & BL_DIGIT_MASK);
                    carry = tmp >> BL_BIT_COUNT;
                }

                if (carry)
                    result[i + size_b] += static_cast<digit>(carry);
            }
            return result;
        }

        static inline digit char_to_num(char ch)
        {
            return (digit)(ch - '0');
        }

        static inline bool is_num_digit(char ch)
        {
            return '0' <= ch && ch <= '9';
        }

        digit extract_digit_from_string(const char *start, const char *end)
        {
            digit accum = 0;
            digit multy = 1;

            long long length = end - start;
            for (long long i = length - 1; i >= 0; i--)
            {
                char ch = start[i];
                if (!is_num_digit(ch))
                    return -1;
                accum += char_to_num(ch) * multy;
                multy *= 10;
            }

            return accum;
        }

        const char *extract_sign_from_string(const char *start, const char *end, sign *signDest)
        {
            *signDest = POSITIVE_NUM;
            const char *cur = start;
            while (cur < end)
            {
                if (*cur == '-')
                {
                    *signDest *= -1;
                    cur++;
                }
                else if (*cur == '+')
                    cur++;
                else
                    break;
            }
            return cur;
        }
    } // namespace _detail

} // namespace bigLong
