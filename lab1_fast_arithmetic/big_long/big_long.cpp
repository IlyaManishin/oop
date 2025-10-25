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

}