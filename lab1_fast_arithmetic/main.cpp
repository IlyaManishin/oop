#include "big_long/big_long.h"

int main()
{
    bigLong::BigLong bl(10);
    bl += 10;
    bl += 100;
    bigLong::BigLong add(1000);
    bl += add;
    bl.printBigLong();
    bl -= add;
    bl.printBigLong();
    bl = bl - add;
    bl.printBigLong();
    return EXIT_SUCCESS;
}