#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "big_long_config.h"
#include "big_long_types.h"

namespace bigLong
{
    class BigLong
    {
    public:
        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        BigLong(T integral);

        template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        BigLong(T floating);

        BigLong(const std::string &numStr);
        BigLong(const char *numStr);

        template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        BigLong operator+(T number);
        BigLong operator+(const BigLong &otherBl);

        template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        BigLong &operator+=(T number);

        BigLong &operator+=(const BigLong &other);

        template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        BigLong &operator-=(T number);

        BigLong &operator-=(const BigLong &other);

        bool operator==(const BigLong &other) const;
        bool operator<(const BigLong &other) const;
        bool operator>(const BigLong &other) const { return other < *this; }

        template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        bool operator<(T other) const { return *this < BigLong(other); }

        template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        bool operator>(T other) const { return *this > BigLong(other); }

        template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        bool operator==(T other) const { return *this == BigLong(other); }

        ~BigLong();

    private:
        std::vector<_detail::digit> digits;
        size_t numSize;
        _detail::sign numSign;

        void initFromString(const char *string, size_t length);
        void initBigLong(size_t digitsCount, _detail::sign numSign);
        void toAbs();
        void swapSign();

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        void initFromIntegral(T integral);

        template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        void initFromLongFloating(T floating);

        void normalize();
        int bigLongAbsCompare(const BigLong &other) const;
    };

}