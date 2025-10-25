#include <cassert>
#include <cmath>
#include <vector>

#include "big_long.h"
#include "big_long_config.h"
#include "big_long_core.h"

namespace bigLong
{
    using namespace _detail;

    void BigLong::initFromString(const char *string, size_t length)
    {
    }

    void BigLong::normalize()
    {
        while (digits.size() > 1 && digits.back() == 0)
            digits.pop_back();

        numSize = digits.size();

        if (numSize == 1 && digits[0] == 0)
            numSign = ZERO_NUM;
    }

    void BigLong::toAbs()
    {
        this->numSign = POSITIVE_NUM;
    }

    void BigLong::swapSign()
    {
        this->numSign = -this->numSign;
    }

    void BigLong::initBigLong(size_t digitsCount, sign numSign)
    {
        this->digits = std::vector<digit>(digitsCount, 0);
        this->numSign = numSign;
    }

    int BigLong::bigLongAbsCompare(const BigLong &other) const
    {
        if (this->numSize > other.numSize)
        {
            return 1;
        }
        else if (this->numSize < other.numSize)
        {
            return -1;
        }
        else
        {
            for (int i = this->numSize - 1; i >= 0; i--)
            {
                if (this->digits[i] < other.digits[i])
                    return -1;
                else if (this->digits[i] > other.digits[i])
                    return 1;
            }
        }
        return 0;
    }

    namespace _detail
    {
        std::vector<digit> abs_digits_add(const std::vector<digit> &num1, const std::vector<digit> &num2)
        {
            size_t minSize = std::min(num1.size(), num2.size());
            size_t maxSize = std::max(num1.size(), num2.size());
            const std::vector<digit> &maxNum = num1.size() > num2.size() ? num1 : num2;

            std::vector<digit> result(maxSize + 1, 0);
            digit carry = 0;
            size_t i = 0;

            for (; i < minSize; ++i)
            {
                digit sum = num1[i] + num2[i] + carry;
                result[i] = sum & BL_DIGIT_MASK;
                carry = sum >> BL_BIT_COUNT;
            }

            for (; i < maxSize; ++i)
            {
                digit sum = maxNum[i] + carry;
                result[i] = sum & BL_DIGIT_MASK;
                carry = sum >> BL_BIT_COUNT;
            }

            if (carry)
                result[i] = carry;

            return result;
        }

        std::vector<digit> abs_digits_sub(const std::vector<digit> &num1, const std::vector<digit> &num2)
        {
            std::vector<digit> result(num1.size(), 0);
            digit borrow = 0;
            size_t i = 0;

            for (; i < num2.size(); ++i)
            {
                digit diff = (digit)num1[i] - num2[i] - borrow;
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
                digit diff = (digit)num1[i] - borrow;
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
    } // namespace _detail

} // namespace bigLong
