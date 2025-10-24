#pragma once

#include "big_long.h"
#include "big_long_config.h"

#include <cassert>
#include <cmath>

namespace bigLong
{
    namespace _detail
    {
        constexpr size_t type_digits_size(size_t byteSize)
        {
            size_t bit_size = byteSize * 8;
            return bit_size % BL_BIT_COUNT == 0 ? bit_size / BL_BIT_COUNT : bit_size / BL_BIT_COUNT + 1;
        }
    } // namespace _detail

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    BigLong::BigLong(T integral)
    {
        this->initFromIntegral(integral);
    }

    template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool>>
    BigLong::BigLong(T floating)
    {
        if (floating > static_cast<long double>(LLONG_MAX) || floating < static_cast<long double>(LLONG_MIN))
        {
            this->initFromLongFloating(floating);
        }
        else
        {
            long long asLong = static_cast<long long>(floating);
            this->initFromIntegral(asLong);
        }
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>>
    BigLong BigLong::operator+(T number)
    {
        BigLong res(*this);
        res += number;
        return res;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>>
    BigLong &BigLong::operator+=(T number)
    {
        if (number == 0)
            return *this;

        BigLong bl(number);
        *this += bl;
        return *this;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>>
    BigLong BigLong::operator-(T number)
    {
        BigLong res(*this);
        res -= number;
        return res;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>>
    BigLong &BigLong::operator-=(T number)
    {
        if (number == 0)
            return *this;

        BigLong bl(number);
        *this -= bl;
        return *this;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>>
    BigLong BigLong::operator*(T number) const
    {
        BigLong res(*this);
        res *= number;
        return res;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool>>
    BigLong &BigLong::operator*=(T number)
    {
        BigLong bl(number);
        *this *= bl;
        return *this;
    }

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    void BigLong::initFromIntegral(T integral)
    {
        if (integral == 0)
        {
            this->initBigLong(0, _detail::ZERO_NUM);
            return;
        }
        _detail::sign numSign = _detail::POSITIVE_NUM;
        if (integral < 0)
        {
            numSign = _detail::NEGATIVE_NUM;
            integral = -integral;
        }
        constexpr size_t digitsCount = _detail::type_digits_size(sizeof(T));
        this->initBigLong(digitsCount, numSign);

        auto unsignedNum = std::make_unsigned_t<T>(integral);
        for (size_t i = 0; i < digitsCount; i++)
        {
            this->digits[i] = unsignedNum & _detail::BL_DIGIT_MASK;
            unsignedNum >>= _detail::BL_BIT_COUNT;
        }
        this->normalize();
    }

    template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool>>
    void BigLong::initFromLongFloating(T floating)
    {
        _detail::sign sign = _detail::POSITIVE_NUM;
        if (floating < 0)
        {
            floating = -floating;
            sign = _detail::NEGATIVE_NUM;
        }

        initBigLong(0, sign);

        long double asCeil;
        std::modf(floating, &asCeil);

        while (asCeil >= 1)
        {
            auto mod = std::fmod(asCeil, static_cast<long double>(_detail::BL_DIGIT_MOD));
            _detail::digit digitPart = static_cast<_detail::digit>(mod);
            digits.push_back(digitPart);
            assert(this->digits[digits.size() - 1] < _detail::BL_DIGIT_MOD);

            asCeil /= _detail::BL_DIGIT_MOD;
        }
    }
} // namespace bigLong
