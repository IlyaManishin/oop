#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "big_long_config.h"
#include "big_long_types.h"

namespace bigLong
{
    using namespace _detail;

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

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        BigLong &operator+=(T integral);
        template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        BigLong &operator+=(T floating);
        BigLong &operator+=(const BigLong &other);

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        BigLong &operator-=(T integral);
        template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        BigLong &operator-=(T floating);
        BigLong &operator-=(const BigLong &other);

        bool operator==(const BigLong &other) const;
        bool operator<(const BigLong &other) const;
        bool operator>(const BigLong &other) const { return other < *this; }

        ~BigLong();

    private:
        std::vector<digit> digits;
        size_t numSize;
        sign numSign;

        void initFromString(const char *string, size_t length);
        void initBigLong(size_t digitsCount = BL_BASE_DIGITS_COUNT, SIGN numSign = POSITIVE_NUM);

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        void initFromIntegral(T integral);

        template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        void initFromLongFloating(T floating);

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        void addIntegral(T integral);

        // template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
        // void addBigFloating(T floating);

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        void subIntegral(T integral);

        void normalize();
        int bigLongAbsCompare(const BigLong &other) const;
    };

}