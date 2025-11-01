#include <cmath>
#include <cstdint>
#include <cstring>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <type_traits>

#include <iostream>

#include "big_long.h"
#include "big_long_config.h"
#include "big_long_core.h"

namespace bigLong
{
    using namespace _detail;

    BigLong::BigLong(const std::string &numStr)
    {
        initFromString(numStr.c_str(), numStr.size());
    }

    BigLong::BigLong(const char *numStr)
    {
        initFromString(numStr, strlen(numStr));
    }

    static std::string invalid_string_msg(const char *str, size_t length)
    {
        return "Invalid string: " + std::string(str, length);
    }

    long long biglong_from_digits_first2(const std::vector<digit> &digits)
    {
        constexpr unsigned int BITS = 30u;
        constexpr unsigned long long MASK = (1ULL << BITS) - 1ULL;

        unsigned long long low = 0ULL;
        unsigned long long high = 0ULL;

        if (digits.size() > 0)
            low = static_cast<unsigned long long>(digits[0]) & MASK;
        if (digits.size() > 1)
            high = static_cast<unsigned long long>(digits[1]) & MASK;

        unsigned long long u = (high << BITS) | low;
        long long result = static_cast<long long>(u);
        return result;
    }

    void BigLong::initFromString(const char *string, size_t length)
    {
        const char *end = string + length;

        sign numSign;
        const char *start = extract_sign_from_string(string, end, &numSign);

        size_t numLength = end - start;
        if (numLength == 0)
        {
            throw std::runtime_error(invalid_string_msg(string, length));
        }

        if (numLength < BL_DEC_IN_DIGIT)
        {
            long long strValue = static_cast<long long>(extract_digit_from_str(start, end));
            this->initFromIntegral(numSign * strValue);
            return;
        }
        this->initZero();

        BigLong accum(1);
        const char *curEnd = end;
        while (curEnd > start)
        {
            const char *digitStart = std::max(curEnd - BL_DEC_IN_DIGIT, start);
            digit dgt = extract_digit_from_str(digitStart, curEnd);
            BigLong add = accum * dgt;
            // std::cout << biglong_from_digits_first2(add.digits) << " + " << biglong_from_digits_first2(this->digits) << std::endl;
            *this = *this + add;
            // std::cout << "=" << biglong_from_digits_first2(this->digits) << std::endl;
            accum *= BL_DEC_PART_DIV;
            curEnd = digitStart;
        }
        this->numSign = numSign;
        this->normalize();
    }

    void BigLong::initEmpty(size_t digitsCount, sign numSign)
    {
        this->digits = std::vector<digit>(digitsCount, 0);
        this->numSign = numSign;
    }

    void BigLong::initZero()
    {
        this->digits.clear();
        this->digits.push_back(0);
        this->numSign = ZERO_NUM;
    }

    size_t BigLong::getSize() const
    {
        return this->numSign == ZERO_NUM ? 0 : this->digits.size();
    }
    size_t BigLong::getSizeDelta(const BigLong &other) const
    {
        if (this->getSize() > other.getSize())
            return this->getSize() - other.getSize();
        else
            return other.getSize() - this->getSize();
    }

    void BigLong::normalize()
    {
        while (digits.size() > 1 && digits.back() == 0)
            digits.pop_back();

        if (digits.size() == 1 && digits[0] == 0)
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

    int BigLong::bigLongAbsCompare(const BigLong &other) const
    {
        if (this->getSize() > other.getSize())
        {
            return 1;
        }
        else if (this->getSize() < other.getSize())
        {
            return -1;
        }
        else
        {
            for (int i = this->getSize() - 1; i >= 0; i--)
            {
                if (this->digits[i] < other.digits[i])
                    return -1;
                else if (this->digits[i] > other.digits[i])
                    return 1;
            }
        }
        return 0;
    }
}