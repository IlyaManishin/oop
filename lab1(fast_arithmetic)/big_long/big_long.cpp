#include <cstring>
#include <inttypes.h>
#include <numeric>
#include <ranges>
#include <type_traits>

#include "big_long.h"
#include "big_long_config.h"
#include "big_long_core.h"

namespace bigLong
{

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    BigLong::BigLong(T num)
    {
        if (num == 0)
        {
            this->numSign = 0;
            this->digits = nullptr;
            return;
        }
        constexpr int digitsSize = digits_size(sizeof(T));

        auto unsignedNum = std::make_unsigned_t<T>(num);
        this->digits = new detail::digit[digitsSize]();
        for (int i = 0; i < digitsSize; i++)
        {
            this->digits[i] = unsignedNum & BL_DIGIT_MASK;
            unsignedNum >>= BL_USED_DIGIT_BITS;
        }
    }

    void BigLong::parseString(const char *string, size_t length)
    {
        BigLong buffer(0);
        for (size_t i = 0; i < length; i++)
        {
            buffer += string[i] - '0';
        }
    }

    BigLong::BigLong(const std::string &numStr)
    {
        parseString(numStr.c_str(), numStr.size());
    }

    BigLong::BigLong(const char *numStr)
    {
        parseString(numStr, strlen(numStr));
    }

    BigLong::~BigLong()
    {
        if (this->digits != nullptr)
            delete this->digits;
    }

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    BigLong BigLong::operator+(T number) {};

    BigLong BigLong::operator+(BigLong &other) {};

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    BigLong BigLong::operator-(T number) {};
    BigLong BigLong::operator-(BigLong &other) {};

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    BigLong BigLong::operator/(T number) {};
    BigLong BigLong::operator/(BigLong &other) {};

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    BigLong BigLong::operator*(T number) {};
    BigLong BigLong::operator*(BigLong &other) {};

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    BigLong &BigLong::operator+=(T number) { return *this; };
    BigLong &BigLong::operator+=(BigLong &other) { return *this; };

}