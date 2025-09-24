#include <vector>

#include "big_long.h"
#include "big_long_core.h"
#include "big_long_config.h"

namespace bigLong
{
    template <typename T, std::enable_if_t<std::is_integral_v<T>, bool>>
    void BigLong::addNumber(T number)
    {
        if (number == 0)
        {
            this->initBigLong(0, ZERO_NUM);
            return;
        }
        sign numSign = number > 0 ? POSITIVE_NUM : NEGATIVE_NUM;
        constexpr int digitsCount = type_digits_size(sizeof(T));
        this->initBigLong(digitsCount);

        auto unsignedNum = std::make_unsigned_t<T>(number);
        for (int i = 0; i < digitsCount; i++)
        {
            this->digits[i] = unsignedNum & BL_DIGIT_MASK;
            unsignedNum >>= BL_USED_DIGIT_BITS;
        }
    }

    void BigLong::initBigLong(size_t digitsCount, SIGN numSign)
    {
        this->digits = std::vector<digit>(digitsCount, 0);
        this->numSign = numSign;
    }

    constexpr int type_digits_size(size_t byteSize)
    {
        return byteSize % BL_USED_DIGIT_BITS == 0 ? byteSize / BL_USED_DIGIT_BITS : byteSize / BL_USED_DIGIT_BITS + 1;
    }

} // namespace bigLong
