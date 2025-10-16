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

    BigLong::~BigLong()
    {
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

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    BigLong &BigLong::operator+=(T integral)
    {
        if (integral > 0)
        {
            this->addIntegral(integral);
        }
        else if (integral < 0)
        {
            auto unsignedNum = std::make_unsigned<T>(integral);
        }
        return *this;
    }

    template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool>>
    BigLong &BigLong::operator+=(T floating)
    {
        *this += BigLong(floating);
        return *this;
    }

    BigLong &BigLong::operator+=(const BigLong &other)
    {
        if (other.numSign == ZERO_NUM) return;

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