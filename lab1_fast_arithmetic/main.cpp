#include "big_long/big_long.h"

int main()
{
    bigLong::BigLong bl(10);
    printf("%d", bl > 9);
    return EXIT_SUCCESS;
}