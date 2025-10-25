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
        if (this->numSign == ZERO_NUM || other.numSign == ZERO_NUM)
        {
            this->digits.clear();
            this->digits.push_back(0);
            this->numSign = ZERO_NUM;
            return *this;
        }
        std::vector<digit> result = abs_digits_mul(this->digits, other.digits);

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