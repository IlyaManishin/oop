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

    BigLong::BigLong(const std::string &numStr)
    {
        initFromString(numStr.c_str(), numStr.size());
    }

    BigLong::BigLong(const char *numStr)
    {
        initFromString(numStr, strlen(numStr));
    }

    void BigLong::initFromString(const char *string, size_t length)
    {
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