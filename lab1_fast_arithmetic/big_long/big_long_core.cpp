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
        for (int i = this->digits.size() - 1; i >= 0; i--)
        {
            if (this->digits[i] != 0)
            {
                this->numSize = i + 1;
                this->digits.resize(this->numSize);
                return;
            }
        }
        this->numSize = 0;
        this->digits.resize(0);
        this->numSign = ZERO_NUM;
    }

    void BigLong::toAbs()
    {
        this->numSign = POSITIVE_NUM;
    }

    void BigLong::swapSign()
    {
        this->numSign = -this->numSign;
    }

    // template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    // void BigLong::addIntegral(T integral)
    // {
    //     assert(integral >= 0);

    //     if (integral == 0)
    //         return;

    //     // constexpr size_t digitsCount = type_digits_size(sizeof(T));
    //     uint64_t uIntegral = (uint64_t)integral;
    //     constexpr int digitsSize = type_digits_size(sizeof(uint64_t));
    //     uint32_t carry = 0;

    //     int i;
    //     for (i = 0; i < digitsSize; i++)
    //     {
    //         if (i >= this->digits.size())
    //             this->digits.push_back(0);
    //         carry = digits[i] + uIntegral & BL_DIGIT_MASK;
    //         digits[i] = carry & BL_DIGIT_MASK;
    //         uIntegral >>= BL_DIGIT_OFFSET;
    //         carry >>= BL_DIGIT_OFFSET;
    //     }
    //     for (; carry > 0; i++)
    //     {
    //         if (i >= this->digits.size())
    //             this->digits.push_back(0);
    //         carry += digits[i];
    //         this->digits[i] = carry & BL_DIGIT_MASK;
    //         carry >>= BL_DIGIT_OFFSET;
    //         i++;
    //     }
    // }

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
    // void BigLong::abs_bl_add(const BigLong &other)
    // {
    //     this->digits = abs_digits_add(this->digits, other.digits);
    //     this->numSize = this->digits.size();
    //     this->normalize();
    // }
    namespace _detail
    {
        std::vector<digit> abs_digits_add(const std::vector<digit> &num1, const std::vector<digit> &num2)
        {
            size_t minSize = std::min(num1.size(), num2.size());
            size_t maxSize = std::max(num1.size(), num2.size());

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
                digit sum = num1[i] + carry;
                result[i] = sum & BL_DIGIT_MASK;
                carry = sum >> BL_BIT_COUNT;
            }

            if (carry)
                result[i++] = carry;

            return result;
        }

        // void BigLong::abs_bl_sub(const BigLong &other)
        // {
        //     assert(this->bigLongAbsCompare(other) >= 0);

        //     this->digits = abs_digits_sub(this->digits, other.digits);
        //     this->numSize = this->digits.size();
        //     this->normalize();
        // }

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
    } // namespace _detail

} // namespace bigLong
