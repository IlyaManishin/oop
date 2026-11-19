#include "big_long/big_long.h"

#include <iostream>

using bigLong::BigLong;

int main()
{
    std::cout << "=== BigLong (Arbitrary Precision Integer) Demo ===\n\n";

    // 1. Initialization
    BigLong a("123456789012");
    BigLong b("987654321098");
    BigLong c = 42;

    std::cout << "Number A: " << a.convertToString() << "\n";
    std::cout << "Number B: " << b.convertToString() << "\n";
    std::cout << "Number C: " << c.convertToString() << "\n\n";

    // 2. Arithmetic Operations
    BigLong sum = a + b;
    BigLong diff = b - a;
    BigLong mul = a * 1000;
    BigLong div = b / a;

    std::cout << "A + B    = " << sum.convertToString() << "\n";
    std::cout << "B - A    = " << diff.convertToString() << "\n";
    std::cout << "A * 1000 = " << mul.convertToString() << "\n";
    std::cout << "B / A    = " << div.convertToString() << "\n\n";

    // 3. Comparisons
    std::cout << "Comparison operations:\n";
    std::cout << "  A < B  : " << (a < b ? "true" : "false") << "\n";
    std::cout << "  B > A  : " << (b > a ? "true" : "false") << "\n";
    std::cout << "  A == A : " << (a == a ? "true" : "false") << "\n\n";

    // 4. Large Numbers & Multi-limb Arithmetic
    std::cout << "Multi-limb calculations (internal 30-bit limbs):\n";
    BigLong bigVal("1000000000000000000"); // 10^18
    bigVal *= 1000;                       // 10^21 (exceeds standard 64-bit int)
    BigLong bigVal2 = bigVal + 5000000;
    std::cout << "  Is (10^21 + 5000000) > 10^21? " << (bigVal2 > bigVal ? "yes" : "no") << "\n";

    std::cout << "\nAll demo calculations finished successfully!\n";
    return EXIT_SUCCESS;
}
