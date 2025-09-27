#include <cassert>
#include <cmath>
#include <vector>

#include "big_long.h"
#include "big_long_config.h"
#include "big_long_core.h"

namespace bigLong
{
    using namespace _detail;

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    void BigLong::initFromIntegral(T integral)
    {
        if (integral == 0)
        {
            this->initBigLong(0, ZERO_NUM);
            return;
        }
        sign numSign = integral > 0 ? POSITIVE_NUM : NEGATIVE_NUM;
        constexpr size_t digitsCount = type_digits_size(sizeof(T));
        this->initBigLong(digitsCount);

        auto unsignedNum = std::make_unsigned_t<T>(integral);
        for (int i = 0; i < digitsCount; i++)
        {
            this->digits[i] = unsignedNum & BL_DIGIT_MASK;
            unsignedNum >>= BL_USED_DIGIT_BITS;
        }
        this->normalize();
    }

    template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool>>
    void BigLong::initFromLongFloating(T floating)
    {
        SIGN sign = POSITIVE_NUM;
        if (floating < 0)
        {
            floating = -floating;
            sign = NEGATIVE_NUM;
        }
        initBigLong(0, sign);

        long double asCeil;
        std::modf(floating, &asCeil);
        
        while (asCeil >= 1)
        {
            digit digitPart = static_cast<digit>(std::fmod(asCeil, (long double)BL_DIGIT_MOD));
            digits.push_back(digitPart);
            assert(this->digits[digitIndex] < BL_DIGIT_MOD);

            asCeil /= BL_DIGIT_MOD;
        }
    }

    void BigLong::initFromString(const char *string, size_t length)
    {
    }

    void BigLong::normalize()
    {
        for (int i = this->digits.size() - 1; i >= 0; i--)
        {
            if (this->digits[i] == 0)
            {
                this->numSize = i + 1;
                return;
            }
        }
        this->numSize = 0;
        this->numSign = ZERO_NUM;
    }

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    void BigLong::addIntegral(T integral)
    {
        assert(integral >= 0);

        if (integral == 0)
            return;

        // constexpr size_t digitsCount = type_digits_size(sizeof(T));
        uint64_t uIntegral = (uint64_t)integral;
        constexpr int digitsSize = type_digits_size(sizeof(uint64_t));
        uint32_t carry = 0;

        int i;
        for (i = 0; i < digitsSize; i++)
        {
            if (i >= this->digits.size())
                this->digits.push_back(0);
            carry = digits[i] + uIntegral & BL_DIGIT_MASK;
            digits[i] = carry & BL_DIGIT_MASK;
            uIntegral >>= BL_DIGIT_OFFSET;
            carry >>= BL_DIGIT_OFFSET;
        }
        for (; carry > 0; i++)
        {
            if (i >= this->digits.size())
                this->digits.push_back(0);
            carry += digits[i];
            this->digits[i] = carry & BL_DIGIT_MASK;
            carry >>= BL_DIGIT_OFFSET;
            i++;
        }
    }

    // template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
    // void addBigFloating(T floating)
    // {
    //     assert(floating > 0);
    //     long double asDouble = static_cast<long double>(floating);
    //     if (asDouble)
    // }

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    void subIntegral(T integral)
    {
    }

    void BigLong::initBigLong(size_t digitsCount, SIGN numSign)
    {
        this->digits = std::vector<digit>(digitsCount, 0);
        this->numSign = numSign;
    }

    constexpr size_t type_digits_size(size_t byteSize)
    {
        return byteSize % BL_USED_DIGIT_BITS == 0 ? byteSize / BL_USED_DIGIT_BITS : byteSize / BL_USED_DIGIT_BITS + 1;
    }

} // namespace bigLong
