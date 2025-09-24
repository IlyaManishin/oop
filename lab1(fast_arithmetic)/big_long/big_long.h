#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "big_long_types.h"
#include "big_long_config.h"

namespace bigLong
{
    class BigLong
    {
    public:
        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        BigLong(T num);
        BigLong(const std::string &numStr);
        BigLong(const char *numStr);

        ~BigLong();

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        BigLong operator+(T number);
        BigLong operator+(BigLong &other);

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        BigLong &operator+=(T number);
        BigLong &operator+=(BigLong &other);

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        BigLong operator-(T number);
        BigLong operator-(BigLong &other);

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        BigLong operator/(T number);
        BigLong operator/(BigLong &other);

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        BigLong operator*(T number);
        BigLong operator*(BigLong &other);

    private:
        std::vector<digit> digits;
        sign numSign;

        void parseString(const char *string, size_t length);
        void initBigLong(size_t digitsCount = BL_BASE_DIGITS_COUNT, SIGN numSign = POSITIVE_NUM);
        
        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        void addNumber(T number);
    };

}