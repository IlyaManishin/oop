#include <cstdint>
#include <cstring>
#include <numeric>
#include <ranges>
#include <type_traits>

#include "big_long.h"
#include "big_long_config.h"
#include "big_long_core.h"

namespace bigLong
{
    using namespace _detail;

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    BigLong::BigLong(T integral)
    {
        initFromIntegral(integral);
    }

    template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool>>
    BigLong::BigLong(T floating)
    {
        if (LONG_LONG_MAX < floating && floating < LONG_LONG_MAX)
        {
            long long asLong = static_cast<long long>(floating);
            initFromIntegral(asLong);
        }
        else
        {
            initFromLongFloating(floating);
        }
    }

    BigLong::BigLong(const std::string &numStr)
    {
        initFromString(numStr.c_str(), numStr.size());
    }

    BigLong::BigLong(const char *numStr)
    {
        initFromString(numStr, strlen(numStr));
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>>
    BigLong BigLong::operator+(T number)
    {
        BigLong res(*this);
        res += number;
        return res;
    }

    BigLong BigLong::operator+(const BigLong &otherBl)
    {
        BigLong res(*this);
        res += otherBl;
        return res;
    };
    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>>
    BigLong &BigLong::operator+=(T number)
    {
        if (number == 0)
            return *this;
        BigLong bl = BigLong(number);
        if (bl > 0)
        {
            *this += bl;
        }
        else
        {
            bl.toAbs();
            *this -= bl;
        }
    }

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

    BigLong::~BigLong()
    {
    }
}