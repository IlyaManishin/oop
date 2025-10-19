#include <cstdint>
#include <cstring>
#include <numeric>
#include <ranges>
#include <type_traits>

#include "big_long.h"
#include "big_long_config.h"
#include "big_long_core.h"
#include "big_long_impl.h"

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

    BigLong BigLong::operator+(const BigLong &otherBl)
    {
        BigLong res(*this);
        res += otherBl;
        return res;
    };

    BigLong &BigLong::operator+=(const BigLong &other)
    {
        if (other.numSign == ZERO_NUM)
            return *this;

        if (this->numSign == ZERO_NUM)
        {
            this->digits = other.digits;
            this->numSize = other.numSize;
            this->numSign = other.numSign;
            return *this;
        }

        if (this->numSign == other.numSign)
        {
            this->digits = abs_digits_add(this->digits, other.digits);
        }
        else
        {
            int cmp = this->bigLongAbsCompare(other);
            if (cmp >= 0)
            {
                this->digits = abs_digits_sub(this->digits, other.digits);
            }
            else
            {
                this->digits = abs_digits_sub(other.digits, this->digits);
                this->swapSign();
            }
        }
        this->normalize();
        return *this;
    }

    BigLong BigLong::operator-(const BigLong &otherBl)
    {
        BigLong res(*this);
        res -= otherBl;
        return res;
    }

    BigLong &BigLong::operator-=(const BigLong &other)
    {
        if (other.numSign == ZERO_NUM)
            return *this;

        if (this->numSign == ZERO_NUM)
        {
            this->digits = other.digits;
            this->numSize = other.numSize;
            this->numSign = -other.numSign;
            return *this;
        }

        if (this->numSign != other.numSign)
        {
            this->digits = abs_digits_add(this->digits, other.digits);
        }
        else
        {
            int cmp = this->bigLongAbsCompare(other);
            if (cmp >= 0)
            {
                this->digits = abs_digits_sub(this->digits, other.digits);
            }
            else
            {
                this->digits = abs_digits_sub(other.digits, this->digits);
                this->swapSign();
            }
        }

        this->normalize();
        return *this;
    }

    BigLong &BigLong::operator*=(const BigLong &other)
    {
        if (this->numSign == 0 || other.numSign == 0)
        {
            this->digits.clear();
            this->numSize = 0;
            this->numSign = 0;
            return *this;
        }

        size_t size_a = this->numSize;
        size_t size_b = other.numSize;
        std::vector<digit> result(size_a + size_b, 0);

        for (size_t i = 0; i < size_a; ++i)
        {
            twodigits carry = 0;
            twodigits f = this->digits[i];

            for (size_t j = 0; j < size_b; ++j)
            {
                twodigits tmp = static_cast<twodigits>(result[i + j]) + f * other.digits[j] + carry;
                result[i + j] = static_cast<digit>(tmp & BL_DIGIT_MASK);
                carry = tmp >> BL_BIT_COUNT;
            }

            if (carry)
                result[i + size_b] += static_cast<digit>(carry);
        }

        this->digits = std::move(result);
        this->numSign *= other.numSign;
        this->normalize();
        return *this;
    }

    bool BigLong::operator<(const BigLong &other) const
    {
        if (this->numSign > other.numSign)
        {
            return false;
        }
        else if (this->numSign < other.numSign)
        {
            return true;
        }
        else
        {
            if (this->numSign == ZERO_NUM)
                return false;
            else if (this->numSign == NEGATIVE_NUM)
                return this->bigLongAbsCompare(other) == 1;
            else
                return this->bigLongAbsCompare(other) == -1;
        }
    }

    bool BigLong::operator==(const BigLong &other) const
    {
        if (this->numSign != other.numSign)
            return false;
        if (this->bigLongAbsCompare(other) != 0)
            return false;
        return true;
    }
}