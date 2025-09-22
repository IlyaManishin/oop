#pragma once

#include <string>

namespace bigLong
{
    namespace detail
    {
        typedef uint digit;
        typedef char sign;
    }
    using namespace detail;

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
        BigLong operator/(BigLong& other);

        template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
        BigLong operator*(T number);
        BigLong operator*(BigLong &other);

    private:
        digit *digits;
        sign numSign;

        void parseString(const char *string, size_t length);
    };

}