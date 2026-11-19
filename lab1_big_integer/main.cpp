#include "big_long/big_long.h"

int main()
{
    bigLong::BigLong bl(10);
    bl *= 10;
    bl *= -123;
    return EXIT_SUCCESS;
}