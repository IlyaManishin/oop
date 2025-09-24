#include <cstring>
#include <cstdint>
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
        
    }

    void BigLong::parseString(const char *string, size_t length)
    {

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
        
    }

    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    BigLong BigLong::operator+(T number){}

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